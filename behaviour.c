/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/31 07:20:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void	ft_usleep(long time_in_ms, t_config *config)
{
	long	start;

	start = get_current_time();
	while (!get_stop(config))
	{
		if (get_current_time() - start >= time_in_ms)
			break ;
		usleep(500);
	}
}

int	taker_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;
	long			time_now;
	long			wait_ms;

	if (get_stop(coder->config))
		return (0);
	pthread_mutex_lock(&dongle->mutex);
	coder->request_time = get_current_time();
	add_waiter(dongle, coder);
	while (!get_stop(coder->config))
	{
		time_now = get_current_time();
		if (!dongle->is_taken
			&& (time_now - dongle->last_released_time
				>= coder->config->dongle_cooldown)
			&& has_highest_priority(dongle, coder))
		{
			dongle->is_taken = 1;
			remove_waiter(dongle, coder);
			pthread_mutex_unlock(&dongle->mutex);
			return (1);
		}
		if (dongle->is_taken || !has_highest_priority(dongle, coder))
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else
		{
			wait_ms = coder->config->dongle_cooldown
				- (time_now - dongle->last_released_time);
			if (wait_ms <= 0)
				wait_ms = 1;
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += wait_ms / 1000;
			ts.tv_nsec += (wait_ms % 1000) * 1000000L;
			if (ts.tv_nsec >= 1000000000L)
			{
				ts.tv_sec += 1;
				ts.tv_nsec -= 1000000000L;
			}
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
	}
	remove_waiter(dongle, coder);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

void	releaser_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_taken = 0;
	dongle->last_released_time = get_current_time();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	is_release(t_coder *coder)
{
	long	refactor_time;

	set_last_compile(coder, get_current_time());
	action_log(coder->config, coder->id, "is compiling");
	ft_usleep(coder->config->time_to_compile, coder->config);
	inc_nbr_compile(coder);
	if (all_coders_done(coder->config->coders))
		set_stop(coder->config, 1);
	releaser_dongle(coder->second_dongle);
	releaser_dongle(coder->first_dongle);
	if (get_stop(coder->config))
		return ;
	action_log(coder->config, coder->id, "is debugging");
	ft_usleep(coder->config->time_to_debug, coder->config);
	if (get_stop(coder->config))
		return ;
	action_log(coder->config, coder->id, "is refactoring");
	if (coder->config->time_to_refactor > 0)
		refactor_time = coder->config->time_to_refactor;
	else if (coder->config->num_coders % 2 != 0
		&& coder->config->time_to_compile * 2 > coder->config->time_to_debug)
		refactor_time = coder->config->time_to_compile * 2
			- coder->config->time_to_debug;
	else
		refactor_time = 0;
	if (refactor_time > 0)
		ft_usleep(refactor_time, coder->config);
}

void	*coder_behaviour(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	if (coder->config->num_coders == 1)
	{
		if (taker_dongle(coder->first_dongle, coder))
			action_log(coder->config, coder->id, "has taken a dongle");
		while (!get_stop(coder->config))
			usleep(1000);
		releaser_dongle(coder->first_dongle);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		usleep(1000);
	while (!get_stop(coder->config))
	{
		if (!taker_dongle(coder->first_dongle, coder))
			break ;
		action_log(coder->config, coder->id, "has taken a dongle");
		if (!taker_dongle(coder->second_dongle, coder))
		{
			releaser_dongle(coder->first_dongle);
			break ;
		}
		action_log(coder->config, coder->id, "has taken a dongle");
		is_release(coder);
	}
	return (NULL);
}

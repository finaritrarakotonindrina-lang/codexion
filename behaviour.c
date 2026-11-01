/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/11/01 08:25:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

static void	wait_cooldown(t_dongle *dongle, t_coder *coder, long now)
{
	struct timespec	ts;
	long			wait_ms;

	wait_ms = coder->config->dongle_cooldown
		- (now - dongle->last_released_time);
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

static int	try_take(t_dongle *dongle, t_coder *coder, long time_now)
{
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
	return (0);
}

int	taker_dongle(t_dongle *dongle, t_coder *coder)
{
	long	time_now;

	if (get_stop(coder->config))
		return (0);
	pthread_mutex_lock(&dongle->mutex);
	coder->request_time = get_current_time();
	add_waiter(dongle, coder);
	while (!get_stop(coder->config))
	{
		time_now = get_current_time();
		if (try_take(dongle, coder, time_now))
			return (1);
		if (dongle->is_taken || !has_highest_priority(dongle, coder))
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else
			wait_cooldown(dongle, coder, time_now);
	}
	remove_waiter(dongle, coder);
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

void	is_release(t_coder *c)
{
	long	refactor;

	set_last_compile(c, get_current_time());
	action_log(c->config, c->id, "is compiling");
	ft_usleep(c->config->time_to_compile, c->config);
	inc_nbr_compile(c);
	if (all_coders_done(c->config->coders))
		set_stop(c->config, 1);
	releaser_dongle(c->second_dongle);
	releaser_dongle(c->first_dongle);
	if (get_stop(c->config))
		return ;
	action_log(c->config, c->id, "is debugging");
	ft_usleep(c->config->time_to_debug, c->config);
	if (get_stop(c->config))
		return ;
	action_log(c->config, c->id, "is refactoring");
	refactor = c->config->time_to_refactor;
	if (refactor == 0 && c->config->num_coders % 2 != 0
		&& c->config->time_to_compile * 2 > c->config->time_to_debug)
		refactor = c->config->time_to_compile * 2 - c->config->time_to_debug;
	if (refactor > 0)
		ft_usleep(refactor, c->config);
}

void	*coder_behaviour(void *args)
{
	t_coder	*c;

	c = (t_coder *)args;
	if (c->config->num_coders == 1)
	{
		taker_dongle(c->first_dongle, c);
		action_log(c->config, c->id, "has taken a dongle");
		while (!get_stop(c->config))
			usleep(1000);
		return (releaser_dongle(c->first_dongle), NULL);
	}
	if (c->id % 2 == 0)
		usleep(1000);
	while (!get_stop(c->config) && taker_dongle(c->first_dongle, c))
	{
		action_log(c->config, c->id, "has taken a dongle");
		if (!taker_dongle(c->second_dongle, c))
			return (releaser_dongle(c->first_dongle), NULL);
		action_log(c->config, c->id, "has taken a dongle");
		is_release(c);
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/27 00:41:37 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void	taker_dongle(t_dongle *dongle, t_coder *coder);
void releaser_dongle(t_dongle *dongle);
void is_release(t_coder *coder);

long get_current_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
void *coder_behaviour(void *args)
{
	t_coder *coder;
	coder = (t_coder *) args;
	while (coder->config->stop != 1)
	{
		if (coder->id == coder->config->num_coders)
		{
			taker_dongle(coder->right_dongle, coder);
			action_log(coder->config, coder->id, "has taken a dongle");
			taker_dongle(coder->left_dongle,  coder);
			action_log(coder->config, coder->id, "has taken a dongle");
			is_release(coder);
		}
		else
		{
			taker_dongle(coder->left_dongle,  coder);
			action_log(coder->config, coder->id, "has taken a dongle");
			taker_dongle(coder->right_dongle, coder);
			action_log(coder->config, coder->id, "has taken a dongle");
			is_release(coder);
		}
	}
	return (NULL);
}
void	taker_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec ts;
	long time_now;
	long wakeup;
	pthread_mutex_lock(&dongle->mutex);
	time_now = get_current_time();
	while (dongle->is_taken == 1
		|| time_now - dongle->last_released_time < coder->config->dongle_cooldown)
	{
		wakeup = dongle->last_released_time + coder->config->dongle_cooldown;
		ts.tv_sec  = wakeup / 1000;
		ts.tv_nsec = (wakeup % 1000) * 1000000L;
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		time_now = get_current_time();
	}
	dongle->is_taken = 1;
	pthread_mutex_unlock(&dongle->mutex);
}

void releaser_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	long current_time;
	current_time = get_current_time();
	dongle->is_taken = 0;
	dongle->last_released_time = current_time;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
void is_release(t_coder *coder)
{
	coder->last_compile_start = get_current_time();
	action_log(coder->config, coder->id, "is compiling");
	usleep(coder->config->time_to_compile * 1000);
	coder->nbr_compile++;
	if(all_coders_done(coder->config->coders))
		coder->config->stop = 1;
	releaser_dongle(coder->left_dongle);
	releaser_dongle(coder->right_dongle);
	action_log(coder->config, coder->id, "is debugging");
	usleep(coder->config->time_to_debug * 1000);
	action_log(coder->config, coder->id, "is refactoring");
	usleep(coder->config->time_to_refactor * 1000);
}


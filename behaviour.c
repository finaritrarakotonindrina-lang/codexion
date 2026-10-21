/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/21 19:29:57 by finarako         ###   ########.fr       */
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
	t_coder *coder = (t_coder *) args;

	if (coder->id == coder->config->num_coders)
	{
		taker_dongle(coder->right_dongle, coder);
		taker_dongle(coder->left_dongle,  coder);
		is_release(coder);
	}
	else
	{
		taker_dongle(coder->left_dongle,  coder);
		taker_dongle(coder->right_dongle, coder);
		is_release(coder);
	}
	return (NULL);
}

// void taker_dongle(t_dongle *dongle, t_config *config, int coder_id)
// {
// 	long current_time;
// 	current_time = get_current_time();
// 	printf("DEBUG: coder %d essaie de prendre dongle %d\n", coder_id, dongle->id);
// 	pthread_mutex_lock(&dongle->mutex);
// 	while (dongle->is_taken == 1 || !(current_time - dongle->last_released_time >= config->dongle_cooldown))
// 	{
// 		pthread_cond_wait(&dongle->cond, &dongle->mutex);
// 		current_time = get_current_time();
// 	}
// 	dongle->is_taken = 1;
// 	printf("DEBUG: dongle %d pris !\n", dongle->id);
// 	pthread_mutex_unlock(&dongle->mutex);
// }


void	taker_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec ts;
	long now;
	long wakeup_ms;

	pthread_mutex_lock(&dongle->mutex);
	now = get_current_time();
	while (dongle->is_taken == 1
		|| now - dongle->last_released_time < coder->config->dongle_cooldown)
	{
		if (dongle->is_taken == 1)
			wakeup_ms = now + 100000;
		else
			wakeup_ms = dongle->last_released_time
				+ coder->config->dongle_cooldown;
		ts.tv_sec  = wakeup_ms / 1000;
		ts.tv_nsec = (wakeup_ms % 1000) * 1000000L;
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		now = get_current_time();
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
	usleep(coder->config->time_to_compile * 1000);
	releaser_dongle(coder->left_dongle);
	releaser_dongle(coder->right_dongle);
	usleep(coder->config->time_to_debug * 1000);
	usleep(coder->config->time_to_refactor * 1000);
}

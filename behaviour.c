/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/20 03:30:59 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void taker_dongle(t_dongle *dongle, t_config *config);
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
	if (coder->id == coder->config->num_coders)
	{
		taker_dongle(coder->right_dongle, coder->config);
		taker_dongle(coder->left_dongle, coder->config);
		is_release(coder);
	}
	else
	{
		taker_dongle(coder->left_dongle, coder->config);
		taker_dongle(coder->right_dongle, coder->config);
		is_release(coder);
	}
	return(NULL);
}

void taker_dongle(t_dongle *dongle, t_config *config)
{
	long current_time;
	current_time = get_current_time();
	pthread_mutex_lock(&dongle->mutex);
	while (dongle->is_taken == 1 || !(current_time - dongle->last_released_time >= config->dongle_cooldown))
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
		current_time = get_current_time();
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

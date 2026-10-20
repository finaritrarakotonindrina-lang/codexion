/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/19 19:43:21 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

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
		coder->last_compile_start = get_current_time();
		usleep(coder->config->time_to_compile * 1000);
		releaser_dongle(coder->left_dongle);
		releaser_dongle(coder->right_dongle);
	}
	else
	{
		taker_dongle(coder->left_dongle, coder->config);
		taker_dongle(coder->right_dongle, coder->config);
		coder->last_compile_start = get_current_time();
		usleep(coder->config->time_to_compile * 1000);
		releaser_dongle(coder->left_dongle);
		releaser_dongle(coder->right_dongle);
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

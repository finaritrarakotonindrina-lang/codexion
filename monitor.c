/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 11:34:18 by finarako          #+#    #+#             */
/*   Updated: 2026/11/01 08:25:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

int	get_stop(t_config *config)
{
	int	stop;

	pthread_mutex_lock(&config->lock);
	stop = config->stop;
	pthread_mutex_unlock(&config->lock);
	return (stop);
}

void	set_stop(t_config *config, int val)
{
	int	i;

	pthread_mutex_lock(&config->lock);
	config->stop = val;
	pthread_mutex_unlock(&config->lock);
	if (val)
	{
		i = 0;
		while (i < config->num_coders)
		{
			pthread_mutex_lock(&config->dongles[i].mutex);
			pthread_cond_broadcast(&config->dongles[i].cond);
			pthread_mutex_unlock(&config->dongles[i].mutex);
			i++;
		}
	}
}

int	all_coders_done(t_coder *coders)
{
	int	i;

	if (coders->config->num_compiles_required <= 0)
		return (0);
	i = 0;
	while (i < coders->config->num_coders)
	{
		if (get_nbr_compile(&coders[i]) < coders->config->num_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_behavior(void *args)
{
	int			i;
	long		the_time;
	t_config	*config;

	config = (t_config *)args;
	while (!get_stop(config))
	{
		if (all_coders_done(config->coders))
			return (set_stop(config, 1), NULL);
		i = -1;
		while (++i < config->num_coders)
		{
			the_time = get_current_time();
			if (the_time - get_last_compile(&config->coders[i])
				>= config->time_to_burnout)
			{
				action_log(config, config->coders[i].id, "burned out");
				return (set_stop(config, 1), NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

void	action_log(t_config *config, int coder_id, char *msg)
{
	pthread_mutex_lock(&config->lock);
	if (!config->stop)
	{
		printf("%ld %d %s\n", get_current_time() - config->time_program_start,
			coder_id, msg);
		if (strcmp(msg, "burned out") == 0)
			config->stop = 1;
	}
	pthread_mutex_unlock(&config->lock);
}

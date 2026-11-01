/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 11:08:50 by finarako          #+#    #+#             */
/*   Updated: 2026/11/01 08:25:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void	init_config(t_config *config)
{
	config->stop = 0;
	pthread_mutex_init(&config->lock, NULL);
}

int	dongles_init(t_config *config)
{
	int	i;

	i = 0;
	config->dongles = malloc(sizeof(t_dongle) * config->num_coders);
	if (!config->dongles)
		return (-1);
	while (i < config->num_coders)
	{
		config->dongles[i].id = i;
		config->dongles[i].is_taken = 0;
		config->dongles[i].last_released_time = 0;
		config->dongles[i].waiter_count = 0;
		pthread_mutex_init(&config->dongles[i].mutex, NULL);
		pthread_cond_init(&config->dongles[i].cond, NULL);
		i++;
	}
	return (0);
}

static void	init_coder_dongles(t_coder *coder, t_config *config, int i)
{
	coder->left_dongle = &config->dongles[(i - 1
			+ config->num_coders) % config->num_coders];
	coder->right_dongle = &config->dongles[i];
	coder->first_dongle = coder->left_dongle;
	coder->second_dongle = coder->right_dongle;
	if (coder->id == config->num_coders)
	{
		coder->first_dongle = coder->right_dongle;
		coder->second_dongle = coder->left_dongle;
	}
}

int	coders_init(t_config *config)
{
	int	i;

	config->coders = malloc(sizeof(t_coder) * config->num_coders);
	if (!config->coders)
		return (-1);
	i = 0;
	while (i < config->num_coders)
	{
		config->coders[i].id = i + 1;
		config->coders[i].nbr_compile = 0;
		config->coders[i].last_compile_start = 0;
		config->coders[i].request_time = 0;
		pthread_mutex_init(&config->coders[i].coder_lock, NULL);
		init_coder_dongles(&config->coders[i], config, i);
		config->coders[i].config = config;
		i++;
	}
	return (0);
}

void	pthread_create_init(t_config *config)
{
	int		i;
	long	start;

	start = get_current_time();
	config->time_program_start = start;
	i = 0;
	while (i < config->num_coders)
	{
		config->coders[i].last_compile_start = start;
		if (pthread_create(&config->coders[i].thread, NULL,
				coder_behaviour, (void *)&config->coders[i]) != 0)
		{
			printf("error creating thread");
			exit(1);
		}
		i++;
	}
}

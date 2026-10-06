/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 11:08:50 by finarako          #+#    #+#             */
/*   Updated: 2026/10/04 14:58:25 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

int dongles_init(t_config *config)
{
	int i;
	i = 0;
	config->dongles = malloc(sizeof(t_dongle) * config->num_coders);
	if (!config->dongles)
		return (-1);
	while (i < config->num_coders)
	{
		config->dongles[i].id = i;
		config->dongles[i].is_taken = 0;
		config->dongles[i].last_released_time = 0;
		pthread_mutex_init(&config->dongles[i].mutex, NULL);
		i++;
	}
	return(0);
}
int coders_init(t_config *config)
{
	int i;
	i = 0;
	config->coders = malloc(sizeof(t_coder) * config->num_coders);
	if (!config->coders)
	{
		free(config->dongles);
		return(-1);
	}
	while (i < config->num_coders)
	{
		config->coders[i].id = i + 1;
		config->coders[i].last_compile_start = 0;
		config->coders[i].left_dongle = &config->dongles[(i - 1 + config->num_coders) % config->num_coders];
		config->coders[i].right_dongle = &config->dongles[(i + 1) % config->num_coders];
		config->coders[i].nbr_compile = 0;
		config->coders[i].config = config;
		i++;
	}
	return (0);
}
void pthread_create_init(t_config *config)
{
	int i;
	i = 0;
		while (i < config->num_coders)
	{
		if (pthread_create(&config.coders[i].thread, NULL, coder_behavior, (void *)&config.coders[i]) != 0)
		{
			printf("error creating thread");
			exit(1);
		}
		i++;	
	}
}

void pthread_join_init(t_config *config)
{
	int j;
	j = 0;
		while(j < config->num_coders)
	{
		pthread_join(config.coders[j].thread, NULL);
		j++;
	}
}

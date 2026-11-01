/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 22:25:18 by finarako          #+#    #+#             */
/*   Updated: 2026/11/01 08:25:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void	cleanup_all(t_config *config)
{
	int	i;

	i = 0;
	while (i < config->num_coders)
	{
		pthread_mutex_destroy(&config->dongles[i].mutex);
		pthread_cond_destroy(&config->dongles[i].cond);
		pthread_mutex_destroy(&config->coders[i].coder_lock);
		i++;
	}
	pthread_mutex_destroy(&config->lock);
	free(config->coders);
	free(config->dongles);
}

void	pthread_join_init(t_config *config)
{
	int	j;

	j = 0;
	while (j < config->num_coders)
	{
		pthread_join(config->coders[j].thread, NULL);
		j++;
	}
}

int	main(int argc, char **argv)
{
	t_config	config;

	parce_arg(argc, argv, &config);
	init_config(&config);
	if (dongles_init(&config) == -1 || coders_init(&config) == -1)
	{
		printf("error allocating memorie");
		exit(1);
	}
	pthread_create_init(&config);
	pthread_create(&config.monitor, NULL, monitor_behavior, (void *)&config);
	pthread_join_init(&config);
	pthread_join(config.monitor, NULL);
	cleanup_all(&config);
	return (0);
}

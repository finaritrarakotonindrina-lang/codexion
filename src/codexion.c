/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 22:25:18 by finarako          #+#    #+#             */
/*   Updated: 2026/11/20 00:21:14 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void	cleanup_all(t_config *config)
{
	int	i;

	i = 0;
	while (config->dongles && i < config->num_coders)
	{
		pthread_mutex_destroy(&config->dongles[i].mutex);
		pthread_cond_destroy(&config->dongles[i].cond);
		i++;
	}
	i = 0;
	while (config->coders && i < config->num_coders)
	{
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

	init_config(&config);
	if (parce_arg(argc, argv, &config) != 0)
		return (1);
	if (config.num_compiles_required == 0)
		return (0);
	if (dongles_init(&config) == -1 || coders_init(&config) == -1)
	{
		cleanup_all(&config);
		return (printf("error allocating memorie\n"), 1);
	}
	if (pthread_create_init(&config) == -1
		|| pthread_create(&config.monitor, NULL, monitor_behavior,
			&config) != 0)
	{
		set_stop(&config, 1);
		cleanup_all(&config);
		return (printf("ERROR creating pthread\n"), 1);
	}
	pthread_join_init(&config);
	pthread_join(config.monitor, NULL);
	cleanup_all(&config);
	return (0);
}

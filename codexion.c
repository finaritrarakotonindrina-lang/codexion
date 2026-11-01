/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 22:25:18 by finarako          #+#    #+#             */
/*   Updated: 2026/10/31 06:40:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

long	get_last_compile(t_coder *coder)
{
	long	val;

	pthread_mutex_lock(&coder->coder_lock);
	val = coder->last_compile_start;
	pthread_mutex_unlock(&coder->coder_lock);
	return (val);
}

void	set_last_compile(t_coder *coder, long val)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_start = val;
	pthread_mutex_unlock(&coder->coder_lock);
}

int	get_nbr_compile(t_coder *coder)
{
	int	val;

	pthread_mutex_lock(&coder->coder_lock);
	val = coder->nbr_compile;
	pthread_mutex_unlock(&coder->coder_lock);
	return (val);
}

void	inc_nbr_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->nbr_compile++;
	pthread_mutex_unlock(&coder->coder_lock);
}

int	main(int argc, char **argv)
{
	t_config	config;
	int			i;

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
	i = 0;
	while (i < config.num_coders)
	{
		pthread_mutex_destroy(&config.dongles[i].mutex);
		pthread_cond_destroy(&config.dongles[i].cond);
		pthread_mutex_destroy(&config.coders[i].coder_lock);
		i++;
	}
	pthread_mutex_destroy(&config.lock);
	free(config.coders);
	free(config.dongles);
	return (0);
}

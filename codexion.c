/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 22:25:18 by finarako          #+#    #+#             */
/*   Updated: 2026/10/25 14:10:53 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

int	main(int argc, char **argv)
{
	int i;
	int j;
	i = 0;
	j = 0;
	t_config	config;
	config.stop = 0;
	parce_arg(argc, argv, &config);
	if(dongles_init(&config) == -1 || coders_init(&config) == -1)
	{
		printf("error allocating memorie");
		exit(1);
	}
	pthread_create_init(&config);
	pthread_create(&config.monitor, NULL, monitor_behavior, (void *)&config);
	pthread_join_init(&config);
	pthread_join(config.monitor, NULL);
	free(config.coders);
	free(config.dongles);
	return (0);
}

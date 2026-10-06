/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 22:25:18 by finarako          #+#    #+#             */
/*   Updated: 2026/10/04 16:44:14 by finarako         ###   ########.fr       */
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
	parce_arg(argc, argv, &config);
	if(dongles_init(&config) == -1 || coders_init(&config) == -1)
	{
		printf("error allocating memorie");
		exit(1);
	}
	pthread_create_init(&config);
	pthread_join_init(&config);
	return (0);
}

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
	i = 0;
	t_config	config;
	parce_arg(argc, argv, &config);
	if(dongles_init(&config) == -1)
	{
		printf("error allocating memorie");
		exit(1);
	}
	if(coders_init(&config) == -1)
	{
		printf("error allocating memorie");
		exit(1);
	}
	// while (i < config.num_coders)
	// {
	// 	printf("coder: %d\n",i + 1);
	// 	printf("id: %d\n", config.coders[i].id);
	// 	printf("compile_start: %ld\n", config.coders[i].last_compile_start);
	// 	printf("left_dobgle: %d\n", config.coders[i].left_dongle->id);
	// 	printf("right_dongle: %d\n", config.coders[i].right_dongle->id);
	// 	printf("number_compile: %d\n", config.coders[i].nbr_compile);
	// 	printf("\n");
	// 	i++;
	// }
	return(0);
}

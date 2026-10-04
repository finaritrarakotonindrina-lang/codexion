/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 22:25:18 by finarako          #+#    #+#             */
/*   Updated: 2026/10/04 15:24:12 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

int	main(int argc, char **argv)
{
	int i;
	i = 0;
	t_config	all_config;
	parce_arg(argc, argv, &all_config);
	if(dongles_init(&all_config) == -1)
	{
		printf("error allocating memorie");
		exit(1);
	}
	if(coders_init(&all_config) == -1)
	{
		printf("error allocating memorie");
		exit(1);
	}
	// while (i < all_config.num_coders)
	// {
	// 	printf("coder: %d\n",i + 1);
	// 	printf("id: %d\n", all_config.coders[i].id);
	// 	printf("compile_start: %ld\n", all_config.coders[i].last_compile_start);
	// 	printf("left_dobgle: %d\n", all_config.coders[i].left_dongle->id);
	// 	printf("right_dongle: %d\n", all_config.coders[i].right_dongle->id);
	// 	printf("number_compile: %d\n", all_config.coders[i].nbr_compile);
	// 	printf("\n");
	// 	i++;
	// }
	return(0);
}

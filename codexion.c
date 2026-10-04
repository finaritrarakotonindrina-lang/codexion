/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 22:25:18 by finarako          #+#    #+#             */
/*   Updated: 2026/10/04 13:31:47 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config	all_config;
	parce_arg(argc, argv, &all_config);
	dongles_init(&all_config);
	coders_init(&all_config);
	printf("GOOD");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/13 03:43:40 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void *coder_behaviour(void *args)
{
	t_coder *coder;
	coder = (t_coder *) args;
	if (coder->id == coder->config->num_coders)
	{
		//il prend right_dongle d’abord, puis left_dongle
	}
	else
	{
		//il prend left_dongle d’abord, puis right_dongle
	}

	printf("the id is : %d\n", coder->id);
	return(NULL);
}


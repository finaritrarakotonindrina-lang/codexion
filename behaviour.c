/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/15 20:21:35 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

void *coder_behaviour(void *args)
{
	// ICI : prendre les deux dongles (dans le bon ordre selon l'id)
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

    // ICI : compiler (par exemple usleep pendant time_to_compile)

    // ICI : relâcher les deux dongles

    // ICI : debugger (usleep pendant time_to_debug)

    // ICI : refactorer (usleep pendant time_to_refactor)

    // et recommencer en boucle...
	printf("the id is : %d\n", coder->id);
	return(NULL);
}


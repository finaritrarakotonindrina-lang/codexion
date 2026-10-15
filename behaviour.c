/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behaviour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 20:05:40 by finarako          #+#    #+#             */
/*   Updated: 2026/10/16 00:16:37 by finarako         ###   ########.fr       */
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

void taker_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	while (dongle->is_taken == 1)
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	dongle->is_taken == 1;
	pthread_mutex_unlock(&dongle->mutex);
}

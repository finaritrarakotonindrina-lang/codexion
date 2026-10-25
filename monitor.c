/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 11:34:18 by finarako          #+#    #+#             */
/*   Updated: 2026/10/25 11:58:19 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

int all_coders_done(t_coder *coders)
{
	int i;
	i = 0;
	while (i < coders->config->num_coders)
	{
		if(coders[i].nbr_compile < coders[i].config->num_compiles_required)
			return(0);
		i++;
	}
	return (1);
}
void *monitor_behavior(void *args)
{
	int i;
	long the_time;
	t_config *config;
	config = (t_config*)args;
	while (config->stop != 1)
	{
		i = 0;
		while (i < config->num_coders)
		{
			the_time = get_current_time();
			if(the_time - config->coders[i].last_compile_start >= config->time_to_burnout)
				config->stop = 1;
			i++;
		}
		usleep(10000);
	}
	return (NULL);
}

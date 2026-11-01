/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 08:25:00 by finarako          #+#    #+#             */
/*   Updated: 2026/11/01 09:42:13 by finarako         ###   ########.fr       */
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

void	ft_usleep(long time_in_ms, t_config *config)
{
	long	start;

	start = get_current_time();
	while (!get_stop(config))
	{
		if (get_current_time() - start >= time_in_ms)
			break ;
		usleep(500);
	}
}

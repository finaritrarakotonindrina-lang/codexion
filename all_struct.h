/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_struct.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:41:54 by finarako          #+#    #+#             */
/*   Updated: 2026/09/27 23:45:20 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALL_STRUCT_H
# define ALL_STRUCT_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	int				is_taken;
	long			last_released_time;
}	t_dongle;

typedef struct s_coder
{
	pthread_t thread;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	int	id;
	int	nbr_compile;
	long	last_compile_start;
	t_config *config;
}	t_coder;

typedef struct s_config
{
	long	num_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	long	num_compiles_required;
	long	dongle_cooldown;
	char	*scheduler;
	int		stop;
	pthread_mutex_t lock;
	t_coder *coder;
	t_dongle *dongle;
}			t_config;

#endif

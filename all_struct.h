/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_struct.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:41:54 by finarako          #+#    #+#             */
/*   Updated: 2026/09/30 20:48:36 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALL_STRUCT_H
# define ALL_STRUCT_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_config	t_config;

typedef enum	e_shedulers
{
	fifo,
	edf
}				t_shedulers;
typedef struct s_dongle
{
	int					id;
	int					is_taken;
	long				last_released_time;
	pthread_mutex_t		mutex;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	int					nbr_compile;
	long				last_compile_start;
	pthread_t			thread;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	t_config			*config;
}						t_coder;

typedef struct s_config
{
	long				num_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	long				num_compiles_required;
	long				dongle_cooldown;
	int					stop;
	t_shedulers			sheduler;
	pthread_mutex_t		lock;
	t_coder				*coders;
	t_dongle			*dongles;
}						t_config;

#endif

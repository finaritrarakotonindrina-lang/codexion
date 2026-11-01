/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:42:26 by finarako          #+#    #+#             */
/*   Updated: 2026/10/31 07:20:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "all_struct.h"

int		main(int argc, char **argv);
long	ft_atol(char *str);
long	is_positive(char *argv);
void	parce_arg(int argc, char **argv, t_config *config);
int		is_sheduler(char *argv);
int		coders_init(t_config *config);
int		dongles_init(t_config *config);
void	pthread_create_init(t_config *config);
void	pthread_join_init(t_config *config);
void	*coder_behaviour(void *args);
int		taker_dongle(t_dongle *dongle, t_coder *coder);
void	releaser_dongle(t_dongle *dongle);
long	get_current_time(void);
void	ft_usleep(long time_in_ms, t_config *config);
void	is_release(t_coder *coder);
int		all_coders_done(t_coder *coders);
void	*monitor_behavior(void *args);
void	action_log(t_config *config, int coder_id, char *msg);
void	init_config(t_config *config);

int		get_stop(t_config *config);
void	set_stop(t_config *config, int val);
long	get_last_compile(t_coder *coder);
void	set_last_compile(t_coder *coder, long val);
int		get_nbr_compile(t_coder *coder);
void	inc_nbr_compile(t_coder *coder);

void	add_waiter(t_dongle *dongle, t_coder *coder);
void	remove_waiter(t_dongle *dongle, t_coder *coder);
int		has_highest_priority(t_dongle *dongle, t_coder *coder);

#endif

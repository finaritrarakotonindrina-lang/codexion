/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:42:26 by finarako          #+#    #+#             */
/*   Updated: 2026/10/26 23:13:21 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

int	main(int argc, char **argv);
long	ft_atol(char *str);
long is_positive(char *argv);
void parce_arg(int argc, char**argv, t_config *all_config);
int is_sheduler(char *argv);
int coders_init(t_config *config);
int dongles_init(t_config *config);
void pthread_create_init(t_config *config);
void pthread_join_init(t_config *config);
void *coder_behaviour(void *args);
void	taker_dongle(t_dongle *dongle, t_coder *coder);
void releaser_dongle(t_dongle *dongle);
long get_current_time();
void is_release(t_coder *coder);
int all_coders_done(t_coder *coders);
void *monitor_behavior(void *args);
void action_log(t_config *config, int coder_id, char *msg);
#endif

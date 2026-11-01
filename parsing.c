/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 21:55:55 by finarako          #+#    #+#             */
/*   Updated: 2026/10/31 06:40:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

long	ft_atol(char *str)
{
	long	i;
	long	result;

	result = 0;
	i = 0;
	if (!str || !*str)
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if (result > (LONG_MAX / 10) || (result == (LONG_MAX / 10)
				&& (str[i] - '0') > (LONG_MAX % 10)))
			return (-1);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

long	is_positive(char *argv)
{
	long	result;

	result = ft_atol(argv);
	if (result < 0)
	{
		printf("ERROR VALUE! THE PROGRAM STOP! SORRY\n");
		exit(1);
	}
	return (result);
}

int	is_sheduler(char *argv)
{
	if (strcmp(argv, "fifo") == 0)
		return (0);
	else if (strcmp(argv, "edf") == 0)
		return (1);
	else
	{
		printf("ERROR VALUE! THE PROGRAM STOP! SORRY\n");
		exit(1);
	}
}

void	parce_arg(int argc, char **argv, t_config *config)
{
	if (argc != 9)
	{
		printf("ERROR VALUE! THE PROGRAM STOP! SORRY\n");
		exit(1);
	}
	config->num_coders = is_positive(argv[1]);
	config->time_to_burnout = is_positive(argv[2]);
	config->time_to_compile = is_positive(argv[3]);
	config->time_to_debug = is_positive(argv[4]);
	config->time_to_refactor = is_positive(argv[5]);
	config->num_compiles_required = is_positive(argv[6]);
	config->dongle_cooldown = is_positive(argv[7]);
	config->sheduler = is_sheduler(argv[8]);
}

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

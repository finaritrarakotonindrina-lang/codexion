/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 07:20:00 by finarako          #+#    #+#             */
/*   Updated: 2026/11/01 08:25:00 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_struct.h"
#include "codexion.h"

static int	is_higher_priority(t_coder *c1, t_coder *c2, t_config *config)
{
	long	deadline1;
	long	deadline2;

	if (!c2)
		return (1);
	if (!c1)
		return (0);
	if (config->sheduler == edf)
	{
		deadline1 = get_last_compile(c1) + config->time_to_burnout;
		deadline2 = get_last_compile(c2) + config->time_to_burnout;
		if (deadline1 < deadline2)
			return (1);
		if (deadline1 > deadline2)
			return (0);
	}
	if (c1->request_time < c2->request_time)
		return (1);
	if (c1->request_time > c2->request_time)
		return (0);
	return (c1->id < c2->id);
}

void	add_waiter(t_dongle *dongle, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < dongle->waiter_count)
	{
		if (dongle->waiters[i] == coder)
			return ;
		i++;
	}
	if (dongle->waiter_count < 4)
	{
		dongle->waiters[dongle->waiter_count] = coder;
		dongle->waiter_count++;
	}
}

void	remove_waiter(t_dongle *dongle, t_coder *coder)
{
	int	i;
	int	j;

	i = 0;
	while (i < dongle->waiter_count)
	{
		if (dongle->waiters[i] == coder)
		{
			j = i;
			while (j < dongle->waiter_count - 1)
			{
				dongle->waiters[j] = dongle->waiters[j + 1];
				j++;
			}
			dongle->waiter_count--;
			return ;
		}
		i++;
	}
}

int	has_highest_priority(t_dongle *dongle, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < dongle->waiter_count)
	{
		if (dongle->waiters[i] != coder
			&& is_higher_priority(dongle->waiters[i], coder, coder->config))
			return (0);
		i++;
	}
	return (1);
}

void	releaser_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_taken = 0;
	dongle->last_released_time = get_current_time();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananarivo.mg>  #+#  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-19 07:42:35 by finarako          #+#    #+#             */
/*   Updated: 2026-09-19 07:42:35 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "all_struct.h"

long	atol(char *str)
{
	long	i;
	long	result;
	result = 0;
	i	= 0;
	while (str[i])
	{
		if(str[i] > '9' && str[i] < '0')
			return (-1);
		else
		{
			result = result * 10 + (str[i] - '0');
		}
		i++;
	}
	return(result);
}

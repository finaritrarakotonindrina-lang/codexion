/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananarivo.mg>  #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-18 19:31:57 by finarako          #+#    #+#             */
/*   Updated: 2026-09-18 19:31:57 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
int main(int argc, char** argv)
{
    if(argc != 7)
        return (0);
    else
        parcing(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
}

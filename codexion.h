/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: finarako <finarako@student.42antananarivo.mg>  #+#  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-09-18 19:30:28 by finarako          #+#    #+#             */
/*   Updated: 2026-09-18 19:30:28 by finarako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CODEXION_H
# define CODEXION_H
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>

struct coder
{
    int id;
    int key;
};

int main(int argc, char** argv);
void parcing(char* a, char* b, char* c, char* d, char* e, char* f, char* g,char* str);

#endif

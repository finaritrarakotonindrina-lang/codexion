# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: finarako <finarako@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/31 06:55:00 by finarako          #+#    #+#              #
#    Updated: 2026/11/23 21:00:00 by finarako         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread -Isrc

SRCDIR		= src
SRCS		= $(SRCDIR)/codexion.c \
			  $(SRCDIR)/parsing.c \
			  $(SRCDIR)/init.c \
			  $(SRCDIR)/behaviour.c \
			  $(SRCDIR)/monitor.c \
			  $(SRCDIR)/scheduler.c \
			  $(SRCDIR)/utils.c

OBJS		= $(SRCS:.c=.o)
HEADERS		= $(SRCDIR)/codexion.h $(SRCDIR)/all_struct.h

RM			= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) a.out

re: fclean all

.PHONY: all clean fclean re

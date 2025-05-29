# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 12:47:29 by kkamei            #+#    #+#              #
#    Updated: 2025/05/02 11:20:02 by kkamei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= libftdprintf.a
SRCS			= ft_dprintf.c ft_dprintstr.c ft_dputnbr_base.c ft_dprintp.c \
				ft_dprintnbr.c ft_dprintchr.c ft_dprintunbr.c \
				ft_dputnbr_lhex.c ft_dputnbr_uhex.c ft_strlen.c
OBJS			= $(SRCS:.c=.o)

CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -Iinclude

all:			$(NAME)

$(NAME):		$(OBJS)
				ar rcs $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

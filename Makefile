# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/26 12:57:26 by kkamei            #+#    #+#              #
#    Updated: 2025/05/26 12:59:00 by kkamei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_NAMES = main.c
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)
CFLAG = -Wall -Werror -Wextra
CFLAG_DEBUG = -g
NAME = minishell

# Additional Libraries
LIBFT_DIR = lib/libft/
FT_DPRINTF_DIR = lib/ft_dprintf
GNL_DIR = lib/get_next_line


all:			$(NAME)

$(NAME): $(OBJS)
	make libft
	make ft_dprintf
	$(CC) $(CFLAG) $(OBJS) \
	-I$(LIBFT_DIR) \
	-I$(FT_DPRINTF_DIR)/include \
	-L$(LIBFT_DIR) -lft \
	-L$(FT_DPRINTF_DIR) -lftdprintf \
	-o $(NAME)

libft:
	make -C $(LIBFT_DIR)

ft_dprintf:
	make -C $(FT_DPRINTF_DIR)

clean:
	$(RM) $(OBJS)
	$(RM) $(BONUS_OBJS)
	make -C $(LIBFT_DIR) clean
	make -C $(FT_DPRINTF_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean
	make -C $(FT_DPRINTF_DIR) fclean

re: fclean $(NAME)

test:
	@test -d $(NAME)_test || git clone git@github.com:M2U7BF/$(NAME)_test.git
	./$(NAME)_test/test.sh

debug: CFLAG += $(CFLAG_DEBUG)
debug: $(NAME)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

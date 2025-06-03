# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/26 12:57:26 by kkamei            #+#    #+#              #
#    Updated: 2025/06/03 10:55:26 by kkamei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_NAMES = exec_vars.c handle_keys.c i_mode_vars.c interactive_mode.c main.c util.c
SRCS = $(addprefix src/, $(SRC_NAMES))
OBJS = $(SRCS:.c=.o)
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
	-lreadline \
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

doc:
	@test -d $(NAME)_doc || git clone git@github.com:M2U7BF/$(NAME)_doc.git

debug: CFLAG += $(CFLAG_DEBUG)
debug: $(NAME)
	ar rcs $(NAME).a $(OBJS)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

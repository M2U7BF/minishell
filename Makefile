# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/26 12:57:26 by kkamei            #+#    #+#              #
#    Updated: 2025/07/03 10:27:52 by kkamei           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_NAMES = exec_vars.c handle_keys.c i_mode_vars.c interactive_mode.c main.c \
						non_interactive_mode.c parse.c util/util.c util/ft_split_leave_separator.c \
						util/ft_multi_split.c token.c exec.c error.c \
						util/ft_multi_split_keep_sep.c proc_unit.c syntax.c \
						util/ft_split_by_word_keep_sep.c util/ft_splitarr_by_word_leave_separator.c \
						util/ft_split_by_words_keep_sep.c util/ft_multi_splitarr_by_word_leave_separator.c \
						util/remove_elem.c here_doc.c file.c util/ft_strtrim_front.c handle_keys_heredoc.c debug.c tokenize.c \
						blank.c quote.c util/token_util.c token_2.c tokenize_2.c tokenize_3.c util/arr_util.c util/str_util.c util/lst_util.c \
						fd.c redirection.c command_path.c proc_unit_2.c pipe.c redirection_2.c
SRCS = $(addprefix src/, $(SRC_NAMES))
OBJS = $(SRCS:.c=.o)
OBJS_NO_MAIN := $(filter-out src/main.o, $(OBJS))
CFLAG = -Wall -Werror -Wextra
CFLAG_DEBUG = -g
NAME = minishell

# Additional Libraries
LIBFT_DIR = lib/libft/
FT_DPRINTF_DIR = lib/ft_dprintf
LIBDEBUG_DIR = $(NAME)_test/ft_libdebug
LIBTEST_DIR = $(NAME)_test/ft_libtest


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
	make -C $(LIBFT_DIR) bonus

ft_dprintf:
	make -C $(FT_DPRINTF_DIR)

libdebug:
	make -C $(LIBDEBUG_DIR)

libtest:
	make -C $(LIBTEST_DIR)

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

prepare_test:
	@test -d $(NAME)_test || git clone git@github.com:M2U7BF/$(NAME)_test.git
	@test -d $(LIBTEST_DIR) || git clone git@github.com:M2U7BF/ft_libtest.git $(LIBTEST_DIR)
	@test -d $(LIBDEBUG_DIR) || git clone git@github.com:M2U7BF/ft_libdebug.git $(LIBDEBUG_DIR)

test:
	@make prepare_test
	./$(NAME)_test/test.sh

test_all:
	@make prepare_test
	./$(NAME)_test/test.sh -a

doc:
	@test -d $(NAME)_doc || git clone git@github.com:M2U7BF/$(NAME)_doc.git

debug: CFLAG += $(CFLAG_DEBUG)
debug: $(OBJS) $(OBJS_NO_MAIN)
	rm -f $(NAME).a
	make libft
	make ft_dprintf
	make libdebug
	make libtest
	$(CC) $(CFLAG) $(OBJS) \
		-I$(LIBFT_DIR) \
		-I$(FT_DPRINTF_DIR)/include \
		-L$(LIBFT_DIR) -lft \
		-L$(FT_DPRINTF_DIR) -lftdprintf \
		-L$(FT_DPRINTF_DIR) -lftdprintf \
		-L$(LIBDEBUG_DIR) -ldebug \
		-lreadline \
		-o $(NAME)
	ar rcs $(NAME).a $(OBJS_NO_MAIN)

re_debug: fclean debug

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

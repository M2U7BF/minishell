/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:02:27 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/04 12:32:18 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/signal.h>

typedef enum e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE
}							t_mode;

// 非対話的モードで変数を保持する構造体
typedef struct s_ni_mode_vars
{
	char					*input_line;
	char					**words;
}							t_ni_mode_vars;

// 対話的モードで変数を保持する構造体
typedef struct s_i_mode_vars
{
	char					*input_line;
	char					*prompt;
	char					**words;
}							t_i_mode_vars;

// 起動情報などを保持する構造体
typedef struct s_exec_vars
{
	t_mode					mode;
	struct s_i_mode_vars	i_vars;
	struct s_ni_mode_vars	ni_vars;
	char					*script;
}							t_exec_vars;

// handle_keys.c
void						ctrl_backslash(int signum);
void						ctrl_c(int signum);
void						handle_signal(void);

// i_mode_vars.c
void						init_i_vars(t_i_mode_vars *i_vars);
void						destroy_i_vars(t_i_mode_vars *vars);

// exec_vars.c
void						init_e_vars(t_exec_vars *e_vars);
int							parse_exec_arg(int argc, char *argv[],
								t_exec_vars *e_vars);

// interactive_mode.c
int							exec_interactive(t_exec_vars *e_vars);

// non_interactive_mode.c
int							exec_non_interactive(t_exec_vars *e_vars);

// parse.c
void						parse(t_i_mode_vars *i_vars);
void						variable_expansion(char ***words);

// util ============================================================

// util.c
void						ft_free(void *p);
char						*ft_strjoin_all(char **words);
void						free_str_array(char **array);

// ft_split_leave_separator.c
char **ft_split_leave_separator(char *s, char separator);

// ft_split_multiple_separators.c
char	**ft_split_multiple_separators(char *s, char *separators);

#endif

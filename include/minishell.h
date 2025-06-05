/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:02:27 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/05 17:14:08 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include "../minishell_test/ft_libdebug/libdebug.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define DEFAULT_BLANK " \t"
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_PERMISSION_DENIED 126

// 起動モード
typedef enum e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE
}							t_mode;

// トークン種別
typedef enum e_token_type
{
	WORD,
	RESERVED_WORD,
	CONTROL_OPERATOR,
}							t_token_type;

typedef struct s_token
{
	char					*str;
	t_token_type			type;
	struct s_token			*next;
}							t_token;

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
	t_token					*token_list;
	int						pro_count;
	pid_t					*child_pids;
	char					**envp;
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
void						init_i_vars(t_i_mode_vars *i_vars, char *envp[]);
void						destroy_i_vars(t_i_mode_vars *vars);

// exec_vars.c
void						init_e_vars(t_exec_vars *e_vars);
int							parse_exec_arg(int argc, char *argv[],
								t_exec_vars *e_vars);

// interactive_mode.c
int							exec_interactive(t_exec_vars *e_vars, char *envp[]);

// non_interactive_mode.c
int							exec_non_interactive(t_exec_vars *e_vars);

// token.c
void						debug_put_token_list(t_token *token_list);
int							debug_put_token_list_compare(t_token *t,
								t_token *t_e);
void						append_token(t_token **token_list, char *str,
								t_token_type type);
t_token						*create_token(char *str, t_token_type type);
t_token						*tokenize(char *input_line);
void						free_token_list(t_token *token_list);
int							is_control_operator(char *s);
int							is_reserved_word(char *s);

// parse.c
char						**word_segmentation(char *input);
void						parse(t_i_mode_vars *i_vars);
void						variable_expansion(t_token **token_list);

// exec.c
int							exec(t_i_mode_vars *i_vars);

// error.c
void						put_error_exit(char *s, int status);

// util ============================================================

// util.c
void						ft_free(void *p);
char						*ft_strjoin_all(char **words);
void						free_str_array(char **array);
int							arrlen(char **arr);

// ft_split_leave_separator.c
char						**ft_split_leave_separator(char *s, char separator);

// ft_split_multiple_separators.c
char						**ft_split_multiple_separators(char *s,
								char *separators);

#endif

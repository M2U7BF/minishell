/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:02:27 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/16 09:55:28 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/ft_dprintf/include/ft_dprintf.h"
# include "../lib/libft/libft.h"
# include "../minishell_test/ft_libdebug/libdebug.h"
# include "get_next_line.h"
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
# include <unistd.h>

# define DEFAULT_BLANK " \t"
# define EXIT_SYNTAX_ERROR 2
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_PERMISSION_DENIED 126
# define REDIRECTION_LIST           \
	{                              \
		">>", "<<", ">", "<", NULL \
	}
# define BLANK_LIST      \
	{                   \
		" ", "\t", NULL \
	}
# define QUOTATION_LIST   \
	{                    \
		"\"", "\'", NULL \
	}
# define TMPFILE_NAME "tmp_here_doc"

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
	BLANK,
	VARIABLE,
	CONTROL_OPERATOR,
	REDIRECTION,
  PIPE,
}							t_token_type;

typedef struct s_token
{
	char					*str;
	t_token_type			type;
	struct s_token			*next;
}							t_token;

typedef enum e_proc_unit_type
{
	SIMPLE_CMD,
  PIPE_LINE,
}							t_proc_unit_type;

// cmd + arg + arg ... を保存する連結リスト。
typedef struct s_proc_unit
{
	t_token					*args;
	t_proc_unit_type		type;
	struct s_proc_unit		*next;
  int in_fd;
  int out_fd;
}							t_proc_unit;

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

// token.c
void						debug_put_token_list(t_token *token_list);
int							debug_put_token_list_compare(t_token *t,
								t_token *t_e);
void						append_token(t_token **token_list, t_token *token);
t_token						*create_token(char *str, t_token_type type);
t_token						*tokenize(char *input_line);
void						free_token_list(t_token *token_list);
int							is_word(char *s);
int							is_control_operator(char *s);
int							is_reserved_word(char *s);
int							is_redirection(char *s);
char						**tokens_to_arr(t_token *token_list);
t_token						*token_dup(t_token *token);
t_token						*join_tokens(t_token *token_list);
void						process_single_quote(t_token *token_list);

// parse.c
int							check_quotation(char *input_line);
void						quote_removal(t_token *token);
void						parse(t_i_mode_vars *i_vars);
void						variable_expansion(t_token **token_list);

// exec.c
int							exec(t_i_mode_vars *i_vars);

// error.c
void						put_error_exit(char *s, int status);

// proc_unit.c
void						debug_put_proc_list(t_proc_unit *proc_list);
void						free_proc_list(t_proc_unit *proc_list);
t_proc_unit					*create_proc_unit(t_token *args,
								t_proc_unit_type type, int in_fd, int out_fd);
void						append_proc_unit(t_proc_unit **proc_list,
								t_proc_unit *proc_unit);

// syntax.c
int							check_syntax_error(t_token *token_list);

// here_doc.c
int							here_doc(char *delimiter);

// file.c
int							is_readable_file(char *pathname);
int							open_additionalfile(char *filename, int *fd);
int							open_outfile(char *filename, int *fd);
int							open_infile(char *filename, int *fd);

// util ============================================================

// util.c
void						ft_free(void *p);
char						*ft_strjoin_all(char **words);
void						free_str_array(char **array);
int							arrlen(char **arr);
int							count_chr(char *s, char c);
char						**lst_to_str_arr(t_list *lst);
void						debug_put_lst(t_list *lst);
void						del_content(void *content);
int							is_include(char *s, char **words);
t_list						*get_prev_lst(t_list **list, t_list *elem);

// ft_multi_split_leave_separator.c
char						**ft_multi_split_leave_separator(char *s,
								char *separator);

// ft_split_leave_separator.c
char						**ft_split_leave_separator(char *s, char separator);

// ft_multi_split.c
char						**ft_multi_split(char *s, char *separators);

// ft_split_by_word_leave_separator.c
char						**ft_split_by_word_leave_separator(char *str,
								char *word);

// TODO: 削除予定
// ft_splitarr_by_word_leave_separator.c
char						**ft_splitarr_by_word_leave_separator(char **arr,
								char *separator);

// ft_multi_split_by_word_leave_separator.c
char						**ft_multi_split_by_word_leave_separator(char *str,
								char **words);

// ft_multi_splitarr_by_word_leave_separator.c
char						**ft_multi_splitarr_by_word_leave_separator(char **arr,
								char **separator);

// ft_splitarr_leave_separator.c
char						**ft_splitarr_leave_separator(char **arr,
								char separator);

// remove_elem.c
char						**remove_elem(char **arr, char **remove_list);

#endif

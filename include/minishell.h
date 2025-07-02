/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:02:27 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 14:12:01 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/ft_dprintf/include/ft_dprintf.h"
# include "../lib/libft/libft.h"
# include "../minishell_test/ft_libdebug/libdebug.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
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
	DELIMITER,
	ERROR,
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
	int						read_fd;
	int						write_fd;
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
	char					*input;
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

// グローバル変数を保持する構造体
typedef struct s_runtime_data
{
	int						exit_status;
	volatile sig_atomic_t	signal;
}							t_runtime_data;

typedef struct s_split_vars
{
	char					*str;
	char					*word;
}							t_split_vars;

extern t_runtime_data		g_vars;

// src ============================================================

// handle_keys.c
void						set_signal_handlers(void);

// i_mode_vars.c
void						init_i_vars(t_i_mode_vars *i_vars);
void						destroy_i_vars(t_i_mode_vars *vars);

// exec_vars.c
void						init_e_vars(t_exec_vars *e_vars);
int							parse_exec_arg(int argc, char *argv[],
								t_exec_vars *e_vars);

// interactive_mode.c
int							exec_interactive(t_i_mode_vars *i_vars);

// non_interactive_mode.c
int							exec_non_interactive(t_exec_vars *e_vars);

// debug.c
void						debug_put_token_list(t_token *token_list);
int							debug_put_token_list_compare(t_token *t,
								t_token *t_e);

// token.c
t_token						*create_token(char *str, t_token_type type);
void						append_token(t_token **token_list, t_token *token);
void						del_token(t_token **token_list, t_token *token);
void						free_token_list(t_token *token_list);
t_token						*token_dup(t_token *token);

// token_2.c
void						insert_token(t_token **first_token, t_token *token,
								t_token *add);
t_token						*get_prev_token(t_token **token_list,
								t_token *token);

// token_util.c
char						**tokens_to_arr(t_token *token_list);
t_token						*join_tokens(t_token *token_list);

// tokenize.c
t_token						*tokenize(char *input_line);

// tokenize_2.c
bool						is_word(char *s);
bool						is_control_operator(char *s);
bool						is_reserved_word(char *s);
bool						is_redirection(char *s);

// tokenize_3.c
bool						is_blank(char *s);

// quote.c
bool						is_quotation_error(char *input_line);
void						quote_removal(t_token *token);
bool						is_quote(char c);

// parse.c
void						parse(t_i_mode_vars *i_vars);
void						variable_expansion(t_token **token_list);

// exec.c
void						reset_redirection(t_list *redirect_fds);
int							open_and_redirect_files(t_proc_unit *current_proc,
								t_list **redirect_fds);
t_list						*pipe_redirect(t_proc_unit *proc,
								t_list *redirect_fds);
t_proc_unit					*process_division(t_token *token_list,
								int *pro_count);
char						**trim_redirection(char ***argv);
int							get_command_path(char **cmd_name);
int							exec(t_i_mode_vars *i_vars);
void						handle_error(int status, char *cmd_path);

// error.c
void						put_error_exit(char *s, int status);
void						libc_error(void);

// proc_unit.c
void						debug_put_proc_list(t_proc_unit *proc_list);
void						free_proc_list(t_proc_unit *proc_list);
t_proc_unit					*create_proc_unit(t_token *args,
								t_proc_unit_type type, int in_fd, int out_fd);
void						append_proc_unit(t_proc_unit **proc_list,
								t_proc_unit *proc_unit);
t_proc_unit					*get_prev_proc(t_proc_unit **proc_list,
								t_proc_unit *proc);

// syntax.c
bool						is_syntax_error(t_token *token_list);

// here_doc.c
int							here_doc(char *delimiter);

// handle_signal_heredoc.c
void						set_heredoc_signal_handlers(void);

// file.c
bool						is_readable_file(char *pathname);
int							open_additionalfile(char *filename, int *fd);
int							open_outfile(char *filename, int *fd);
int							open_infile(char *filename, int *fd);

// blank.c
void						remove_blank(t_token *token_list);
void						process_str_quoted(t_token *token_list);

// src/util ============================================================

// util.c
void						ft_free(void **p);

// arr_util.c
char						*ft_strjoin_all(char **words);
void						free_str_array(char **array);
int							arrlen(char **arr);

// str_util.c
int							count_chr(char *s, char c);
bool						is_include(char *s, char **words);
bool						is_str_equal(char *s1, char *s2,
								bool include_null_char);

// lst_util.c
char						**lst_to_str_arr(t_list *lst);
void						debug_put_lst(t_list *lst);
void						del_content(void *content);
t_list						*get_prev_lst(t_list **list, t_list *elem);

// ft_strtrim_front.c
void						ft_strtrim_front(char **s1, char *set);

// ft_multi_split_leave_separator.c
char						**ft_multi_split_leave_separator(char *s,
								char *separator);

// ft_split_leave_separator.c
char						**ft_split_leave_separator(char *s, char separator);

// ft_multi_split.c
char						**ft_multi_split(char *s, char *separators);

// ft_split_by_word_keep_sep.c
char						**ft_split_by_word_keep_sep(char *str, char *word);

// ft_splitarr_by_word_leave_separator.c
char						**ft_splitarr_by_word_leave_separator(char **arr,
								char *separator);

// ft_split_by_words_keep_sep.c
char						**ft_split_by_words_keep_sep(char *str,
								char **words);

// ft_multi_splitarr_by_word_leave_separator.c
char						**ft_multi_splitarr_by_word_leave_separator(char **arr,
								char **separator);

// remove_elem.c
char						**remove_elem(char **arr, char **remove_list);

#endif

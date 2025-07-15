/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:47:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 10:05:57 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	null_to_empty(t_token *token)
{
	t_token	*current_token;

	current_token = token;
	while (current_token)
	{
		if (!current_token->str)
			current_token->str = ft_strdup("");
		current_token = current_token->next;
	}
}

char	*expand_heredoc_line(char *line)
{
	t_token	*token;
	char	**tmp_arr;
	char	*tmp_str;

	if (line && line[0] == '\0')
		return (line);
	token = tokenize(line);
	variable_expansion(&token);
	quote_removal(token);
	null_to_empty(token);
	tmp_arr = tokens_to_arr(token);
	free_token_list(&token);
	tmp_str = ft_strjoin_all(tmp_arr);
	free_str_array(&tmp_arr);
	return (tmp_str);
}

char	*str_quote_removal(char *s)
{
	char	**tmp_arr;
	char	*tmp_str;
	t_token	*tmp_token;

	tmp_token = tokenize(s);
	quote_removal(tmp_token);
	tmp_arr = tokens_to_arr(tmp_token);
	free_token_list(&tmp_token);
	tmp_str = ft_strjoin_all(tmp_arr);
	free_str_array(&tmp_arr);
	ft_free((void **)&s);
	return (tmp_str);
}

static void	finish_here_doc(char **line, char **delim, int *pipe_fds)
{
	ft_free((void **)line);
	ft_free((void **)delim);
	if (close(pipe_fds[1]) == -1)
		libc_error();
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		libc_error();
}

// ヒアドキュメントの処理。
// 入力データはパイプによって、カーネルにバッファリングされる。
int	here_doc(char *delim, int *fd)
{
	char	*line;
	int		pipe_fds[2];
	int		i;

	i = 0;
	set_heredoc_signal_handlers();
	if (is_quoted(delim))
		delim = str_quote_removal(delim);
	if (pipe(pipe_fds) == -1)
		libc_error();
	while (1)
	{
		line = readline("> ");
		if (!line)
			ft_dprintf(STDERR_FILENO, WARN_HEREDOC_1, i, delim);
		if (g_vars.interrupted || !line || is_s_eq(line, delim, true))
			break ;
		if (!is_quoted(delim))
			line = expand_heredoc_line(line);
		ft_dprintf(pipe_fds[1], "%s\n", line);
		ft_free((void **)&line);
		i++;
	}
	finish_here_doc(&line, &delim, pipe_fds);
	if (g_vars.interrupted)
	{
		if (close(pipe_fds[0]) == -1)
			libc_error();
		g_vars.interrupted = 0;
		return (128 + SIGINT);
	}
	*fd = pipe_fds[0];
	return (EXIT_SUCCESS);
}

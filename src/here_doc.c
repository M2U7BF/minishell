/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:47:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/30 09:46:19 by kkamei           ###   ########.fr       */
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
	free_token_list(token);
	tmp_str = ft_strjoin_all(tmp_arr);
	free_str_array(tmp_arr);
	return (tmp_str);
}

// ヒアドキュメントの処理。
// 入力データはパイプによって、カーネルにバッファリングされる。
int	here_doc(char *delimiter)
{
	char	*line;
	int		pipe_fds[2];
	bool	is_quoted;
	t_token	*tmp_token;
	char	**tmp_arr;
	char	*tmp_str;

	handle_signal_heredoc();
	delimiter = ft_strdup(delimiter);
	is_quoted = ft_strchr(delimiter, '\"') != NULL || ft_strchr(delimiter,
			'\'') != NULL;
	if (is_quoted)
	{
		tmp_token = tokenize(delimiter);
		quote_removal(tmp_token);
		tmp_arr = tokens_to_arr(tmp_token);
		free_token_list(tmp_token);
		tmp_str = ft_strjoin_all(tmp_arr);
		free_str_array(tmp_arr);
		ft_free((void **)&delimiter);
		delimiter = tmp_str;
	}
	if (pipe(pipe_fds) == -1)
		libc_error();
	g_runtime_data.signal = 0;
	while (1)
	{
		line = readline("> ");
		if (g_runtime_data.signal == SIGINT || !line || ft_strncmp(line,
				delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		if (!is_quoted)
			line = expand_heredoc_line(line);
		ft_dprintf(pipe_fds[1], "%s\n", line);
		ft_free((void **)&line);
	}
	ft_free((void **)&line);
	ft_free((void **)&delimiter);
	if (close(pipe_fds[1]) == -1)
		libc_error();
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		libc_error();
	return (pipe_fds[0]);
}

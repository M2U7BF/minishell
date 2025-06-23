/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:47:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/24 08:43:26 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_heredoc_line(char *line)
{
	t_token	*token;
	char	**tmp_arr;
	char	*tmp_str;

	token = tokenize(line);
	variable_expansion(&token);
	quote_removal(token);
	tmp_arr = tokens_to_arr(token);
	free_token_list(token);
	tmp_str = ft_strjoin_all(tmp_arr);
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
		delimiter = tmp_str;
	}
	pipe(pipe_fds);
	while (1)
	{
		line = readline("> ");
    if (!line)
      line = ft_strdup("");
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			ft_free(line);
			ft_free(delimiter);
			break ;
		}
		if (!is_quoted)
      line = expand_heredoc_line(line);
    ft_dprintf(pipe_fds[1], "%s\n", line);
		ft_free(line);
	}
	close(pipe_fds[1]);
  signal(SIGINT, SIG_IGN);
	return (pipe_fds[0]);
}

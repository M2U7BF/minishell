/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:47:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 16:01:37 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	null_to_empty(t_token *token)
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

static int	finish_here_doc(char **line, char **delim, int out_fd)
{
	ft_free((void **)line);
	ft_free((void **)delim);
	if (close(out_fd) == -1)
		libc_error();
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		libc_error();
	return (EXIT_SUCCESS);
}

static char	*input_line(void)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		line = get_next_line(STDIN_FILENO);
		trim_endl(&line);
	}
	return (line);
}

// Processing of here-document.
// Input data is buffered by the kernel through a pipe.
int	here_doc(char *delim, int out_fd)
{
	char	*line;
	int		i;
	bool	is_delim_quoted;

	i = 1;
	set_heredoc_signal_handlers();
	is_delim_quoted = is_quoted(delim);
	update_delim(&delim, is_delim_quoted);
	while (1)
	{
		line = input_line();
		if (!line)
			ft_dprintf(STDERR_FILENO, WARN_HEREDOC_1, i, delim);
		if (g_vars.interrupted || !line || is_s_eq(line, delim, true))
			break ;
		if (!is_delim_quoted)
			line = expand_heredoc_line(line);
		ft_dprintf(out_fd, "%s\n", line);
		if (line)
			ft_free((void **)&line);
		i++;
	}
	return (finish_here_doc(&line, &delim, out_fd));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:47:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/12 12:27:00 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_delimiter(char *line, char *delimiter)
{
	size_t	line_len;
	size_t	delim_len;
	int		is_same_chars;

	line_len = ft_strlen(line);
	delim_len = ft_strlen(delimiter);
	is_same_chars = ft_strncmp(line, delimiter, delim_len) == 0 && line_len
		- 1 == delim_len;
	return (is_same_chars);
}

static void	free_gnl(char *line)
{
	free(line);
	get_next_line(0, 1);
}

int	here_doc(char *delimiter, char *tmp_filename)
{
	int		out_fd;
	char	*line;
	int		status;

	status = open_outfile(tmp_filename, &out_fd);
	if (status != 0)
		exit(status);
	while (1)
	{
    ft_dprintf(STDERR_FILENO, "> ");
		line = get_next_line(STDIN_FILENO, 0);
		if (is_delimiter(line, delimiter))
		{
			free_gnl(line);
			break ;
		}
		if (write(out_fd, line, ft_strlen(line)) < 0)
			return (free_gnl(line), close(out_fd), -1);
		free_gnl(line);
	}
	close(out_fd);
	status = open_infile(tmp_filename, &out_fd);
	if (status != 0)
		exit(status);
	return (out_fd);
}

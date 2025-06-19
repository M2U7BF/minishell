/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:45:39 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/19 13:33:28 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_readable_file(char *pathname)
{
	int		fd;
	char	buf;
	ssize_t	bytes;

	fd = open(pathname, O_RDONLY);
	if (fd == -1)
		return (false);
	bytes = read(fd, &buf, true);
	if (bytes == -1)
		return (close(fd), false);
	return (close(fd), true);
}

int	open_additionalfile(char *filename, int *fd)
{
	if (filename[0] == '\0')
		return (ENOENT);
	if (access(filename, F_OK) == 0)
	{
		if (access(filename, R_OK) == -1)
			return (EACCES);
		if (!is_readable_file(filename))
			return (EISDIR);
	}
	*fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (*fd == -1)
		return (EXIT_FAILURE);
	return (0);
}

int	open_outfile(char *filename, int *fd)
{
	if (filename[0] == '\0')
		return (ENOENT);
	if (access(filename, F_OK) == 0)
	{
		if (access(filename, R_OK) == -1)
			return (EACCES);
		if (!is_readable_file(filename))
			return (EISDIR);
	}
	*fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (*fd == -1)
		return (EXIT_FAILURE);
	return (0);
}

int	open_infile(char *filename, int *fd)
{
	if (access(filename, F_OK) == -1)
		return (ENOENT);
	if (access(filename, R_OK) == -1)
		return (EACCES);
	*fd = open(filename, O_RDONLY);
	if (*fd == -1)
		return (EXIT_FAILURE);
	return (0);
}

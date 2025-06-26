/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:45:39 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/26 09:12:57 by kkamei           ###   ########.fr       */
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
	{
		if (close(fd) == -1)
			libc_error();
		return (false);
	}
	if (close(fd) == -1)
		libc_error();
	return (true);
}

int	open_additionalfile(char *filename, int *fd)
{
	*fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (*fd == -1)
		return (errno);
	return (0);
}

int	open_outfile(char *filename, int *fd)
{
	*fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (*fd == -1)
		return (errno);
	return (0);
}

int	open_infile(char *filename, int *fd)
{
	*fd = open(filename, O_RDONLY);
	if (*fd == -1)
		return (errno);
	return (0);
}

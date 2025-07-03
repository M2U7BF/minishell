/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:16:41 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 14:16:57 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_invalid_fd(int fd)
{
	struct stat	sb;

	if (fstat(fd, &sb) == -1 && errno == EBADF)
		return (true);
	return (false);
}

int	stashfd(int fd)
{
	int	stashfd;

	if (fd == -1)
		return (-1);
	stashfd = 10;
	while (1)
	{
		if (is_invalid_fd(stashfd))
			break ;
		stashfd++;
	}
	if (dup2(fd, stashfd) == -1)
		libc_error();
	if (close(fd) == -1)
		libc_error();
	return (stashfd);
}

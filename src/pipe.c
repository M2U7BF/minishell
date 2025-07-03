/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:27:49 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 17:27:58 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_pipe(t_proc_unit *proc)
{
	if (proc->read_fd != STDIN_FILENO)
	{
		if (close(proc->read_fd) == -1)
			libc_error();
	}
	if (proc->write_fd != STDOUT_FILENO)
	{
		if (close(proc->write_fd) == -1)
			libc_error();
	}
}

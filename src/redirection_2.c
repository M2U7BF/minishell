/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:26:41 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/16 10:56:48 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*pipe_redirect(t_proc_unit *proc, t_list *redirect_fds)
{
	if (!proc)
		return (redirect_fds);
	if (proc->read_fd != STDIN_FILENO)
  {
    ft_dprintf(STDERR_FILENO, "pipe_redirect:read:redirect\n");
		redirect(&proc->read_fd, STDIN_FILENO, &redirect_fds);
  }
	if (proc->write_fd != STDOUT_FILENO)
  {
    ft_dprintf(STDERR_FILENO, "pipe_redirect:write:redirect\n");
		redirect(&proc->write_fd, STDOUT_FILENO, &redirect_fds);
  }
	return (redirect_fds);
}

int	get_to_fd(char *str)
{
	if (str[0] == '>')
		return (STDOUT_FILENO);
	return (STDIN_FILENO);
}

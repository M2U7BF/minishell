/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:26:41 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/16 13:26:30 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*pipe_redirect(t_proc_unit *proc)
{
	if (!proc)
		return (proc->redirect_fds);
	if (proc->read_fd != STDIN_FILENO)
		redirect(&proc->read_fd, STDIN_FILENO, &proc->redirect_fds);
	if (proc->write_fd != STDOUT_FILENO)
		redirect(&proc->write_fd, STDOUT_FILENO, &proc->redirect_fds);
	return (proc->redirect_fds);
}

int	get_to_fd(char *str)
{
	if (str[0] == '>')
		return (STDOUT_FILENO);
	return (STDIN_FILENO);
}

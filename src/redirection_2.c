/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:26:41 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 10:34:46 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*pipe_redirect(t_proc_unit *proc, t_list *redirect_fds)
{
	int	target_fd;
	int	*content;

	if (!proc)
		return (redirect_fds);
	target_fd = 0;
	if (proc->read_fd != STDIN_FILENO)
	{
		content = malloc(sizeof(int) * 2);
		if (!content)
		{
			ft_lstclear(&redirect_fds, del_content);
			return (NULL);
		}
    redirect(&proc->read_fd, STDIN_FILENO, &redirect_fds);
	}
	if (proc->write_fd != STDOUT_FILENO)
	{
		content = malloc(sizeof(int) * 2);
		if (!content)
		{
			ft_lstclear(&redirect_fds, del_content);
			libc_error();
		}
    redirect(&proc->write_fd, STDOUT_FILENO, &redirect_fds);
	}
	return (redirect_fds);
}

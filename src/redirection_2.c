/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:26:41 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 10:27:01 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list	*pipe_redirect(t_proc_unit *proc, t_list *redirect_fds)
{
	int	target_fd;
	int	stashed_target_fd;
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
		proc->read_fd = stashfd(proc->read_fd);
		target_fd = STDIN_FILENO;
		stashed_target_fd = stashfd(target_fd);
		if (dup2(proc->read_fd, target_fd) == -1)
			libc_error();
		if (close(proc->read_fd) == -1)
			libc_error();
		content[0] = stashed_target_fd;
		content[1] = target_fd;
		ft_lstadd_back(&redirect_fds, ft_lstnew((void *)content));
	}
	if (proc->write_fd != STDOUT_FILENO)
	{
		content = malloc(sizeof(int) * 2);
		if (!content)
		{
			ft_lstclear(&redirect_fds, del_content);
			libc_error();
		}
		proc->write_fd = stashfd(proc->write_fd);
		target_fd = STDOUT_FILENO;
		stashed_target_fd = stashfd(target_fd);
		if (dup2(proc->write_fd, target_fd) == -1)
			libc_error();
		if (close(proc->write_fd) == -1)
			libc_error();
		content[0] = stashed_target_fd;
		content[1] = target_fd;
		ft_lstadd_back(&redirect_fds, ft_lstnew((void *)content));
	}
	return (redirect_fds);
}

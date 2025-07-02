/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:19:10 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 14:20:36 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// 必要なfileをopenし、リダイレクトを行う。
int	open_and_redirect_files(t_proc_unit *current_proc, t_list **redirect_fds)
{
	int		fd;
	int		to_fd;
	int		stashed_to_fd;
	t_token	*current;
	int		*content;
	int		status;

	fd = 0;
	status = 0;
	current = current_proc->args;
	to_fd = STDIN_FILENO;
	while (current && current->next)
	{
		if (current->type == REDIRECTION && (current->next->type == WORD
				|| current->next->type == DELIMITER))
		{
			if (!current->next->str)
			{
				ft_dprintf(STDERR_FILENO, "minishell: ambiguous redirect\n");
				return (EXIT_FAILURE);
			}
			if (current->str[0] == '>')
				to_fd = STDOUT_FILENO;
			if (is_str_equal(current->str, ">", 1))
				status = open_outfile(current->next->str, &fd);
			else if (is_str_equal(current->str, "<", 1))
				status = open_infile(current->next->str, &fd);
			else if (is_str_equal(current->str, ">>", 1))
				status = open_additionalfile(current->next->str, &fd);
			else if (is_str_equal(current->str, "<<", 1))
				fd = here_doc(ft_strdup(current->next->str));
			if (status != 0)
			{
				handle_error(status, current->next->str);
				return (status);
			}
			fd = stashfd(fd);
			stashed_to_fd = stashfd(to_fd);
			if (fd != to_fd && (dup2(fd, to_fd) == -1 || close(fd) == -1))
				libc_error();
			content = malloc(sizeof(int) * 2);
			if (!content)
				return (EXIT_FAILURE);
			content[0] = stashed_to_fd;
			content[1] = to_fd;
			ft_lstadd_back(redirect_fds, ft_lstnew((void *)content));
			current = current->next;
		}
		current = current->next;
	}
	return (status);
}

char	**trim_redirection(char ***argv)
{
	int		i;
	char	**new;
	t_list	*tmp_lst;
	int		len;

	if (!argv || !(*argv))
		return (NULL);
	i = 0;
	tmp_lst = NULL;
	len = arrlen((*argv));
	while (i < len)
	{
		if (is_redirection((*argv)[i]) && (*argv)[i + 1])
		{
			i += 2;
			continue ;
		}
		ft_lstadd_back(&tmp_lst, ft_lstnew((void *)ft_strdup((*argv)[i])));
		i++;
	}
	free_str_array((*argv));
	new = lst_to_str_arr(tmp_lst);
	ft_lstclear(&tmp_lst, del_content);
	return (new);
}

void	reset_redirection(t_list *redirect_fds)
{
	t_list	*current;
	int		*content;

	if (!redirect_fds)
		return ;
	current = redirect_fds;
	while (current->next)
		current = current->next;
	while (1)
	{
		content = (int *)current->content;
		if (dup2(content[0], content[1]) == -1)
			libc_error();
		if (close(content[0]) == -1)
			libc_error();
		if (current == redirect_fds)
			break ;
		current = get_prev_lst(&redirect_fds, current);
	}
	ft_lstclear(&redirect_fds, del_content);
}

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

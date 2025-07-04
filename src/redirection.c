/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:19:10 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/04 13:28:57 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	open_files(t_token *current, int *status, int *fd)
{
	if (is_str_equal(current->str, ">", true))
		*status = open_outfile(current->next->str, fd);
	else if (is_str_equal(current->str, "<", true))
		*status = open_infile(current->next->str, fd);
	else if (is_str_equal(current->str, ">>", true))
		*status = open_additionalfile(current->next->str, fd);
	else if (is_str_equal(current->str, "<<", true))
		*fd = here_doc(ft_strdup(current->next->str));
}

void	redirect(int *fd, int to_fd, t_list **redirect_fds)
{
	int	*content;
	int	stashed_to_fd;

	*fd = stashfd(*fd);
	stashed_to_fd = stashfd(to_fd);
	if (*fd != to_fd && (dup2(*fd, to_fd) == -1 || close(*fd) == -1))
		libc_error();
	content = malloc(sizeof(int) * 2);
	if (!content)
		libc_error();
	content[0] = stashed_to_fd;
	content[1] = to_fd;
	ft_lstadd_back(redirect_fds, ft_lstnew((void *)content));
}

// 必要なfileをopenし、リダイレクトを行う。
int	open_and_redirect_files(t_token *cur, t_list **redirect_fds)
{
	int	fd;
	int	status;
	int	to_fd;

	fd = 0;
	status = 0;
	to_fd = STDIN_FILENO;
	while (cur && cur->next)
	{
		if (cur->type == REDIR && (cur->next->type == WORD
				|| cur->next->type == DELIM))
		{
			if (!cur->next->str)
				return (ft_dprintf(STDERR_FILENO, ERR_REDIR_1), EXIT_FAILURE);
			open_files(cur, &status, &fd);
			if (status != 0)
				return (handle_error(status, cur->next->str), status);
			if (cur->str[0] == '>')
				to_fd = STDOUT_FILENO;
			redirect(&fd, to_fd, redirect_fds);
			cur = cur->next;
		}
		cur = cur->next;
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

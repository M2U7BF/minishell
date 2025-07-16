/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:19:10 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/16 16:57:55 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	open_files(t_token *current, int *status, int *fd, char *tmp_path)
{
	if (is_s_eq(current->str, ">", true))
		*status = open_outfile(current->next->str, fd);
	else if (is_s_eq(current->str, "<", true))
		*status = open_infile(current->next->str, fd);
	else if (is_s_eq(current->str, ">>", true))
		*status = open_additionalfile(current->next->str, fd);
	else if (is_s_eq(current->str, "<<", true))
		*status = open_infile(tmp_path, fd);
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

// Open the required files and perform redirection
int	open_and_redirect_files(t_token *cur_t, t_proc_unit *cur_p)
{
	int	fd;
	int	status;
	int	heredoc_ind;

	heredoc_ind = 0;
	fd = 0;
	status = 0;
	while (cur_t && cur_t->next)
	{
		if (is_redir_pair(cur_t))
		{
			if (!cur_t->next->str)
				return (ft_dprintf(STDERR_FILENO, ERR_REDIR_1), EXIT_FAILURE);
			open_files(cur_t, &status, &fd,
				cur_p->heredoc_tmp_paths[heredoc_ind]);
			if (status != 0)
				return (handle_error(status, cur_t->next->str), status);
			if (cur_t->next->type == DELIM)
				heredoc_ind++;
      redirect(&fd, get_to_fd(cur_t->str), &cur_p->redirect_fds);
			cur_t = cur_t->next;
		}
		cur_t = cur_t->next;
	}
	return (status);
}

char	**trim_redirection(char ***argv)
{
	int		i;
	char	**new;
	t_list	*tmp_lst;
	int		len;

	if (!argv || !*argv)
		return (NULL);
	i = 0;
	tmp_lst = NULL;
	len = arrlen(*argv);
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
	free_str_array(argv);
	new = lst_to_str_arr(tmp_lst);
	ft_lstclear(&tmp_lst, del_content);
	return (new);
}

void	reset_redirection(t_list **redirect_fds)
{
	t_list	*current;
	int		*content;

	if (!redirect_fds || !*redirect_fds)
		return ;
	current = *redirect_fds;
	while (current->next)
		current = current->next;
	while (1)
	{
		content = (int *)current->content;
		if (dup2(content[0], content[1]) == -1)
			libc_error();
		if (close(content[0]) == -1)
			libc_error();
		if (current == *redirect_fds)
			break ;
		current = get_prev_lst(redirect_fds, current);
	}
	ft_lstclear(redirect_fds, del_content);
}

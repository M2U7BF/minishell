/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:20:37 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 08:45:37 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_delim(char **delim, bool is_delim_quoted)
{
	if (is_delim_quoted)
		*delim = str_quote_removal(*delim);
}

char	*get_tmp_file_path(int i, int *j)
{
	char		*tmp_file_path;
	static char	*pathname_arr[] = {TMP_PATH, "", "_", "", NULL};

	pathname_arr[1] = ft_itoa(i);
	pathname_arr[3] = ft_itoa(*j);
	tmp_file_path = ft_strjoin_all(pathname_arr);
	ft_free((void **)&pathname_arr[1]);
	ft_free((void **)&pathname_arr[3]);
	return (tmp_file_path);
}

static int	inner_process(t_proc_unit *cur_p, t_token *cur_t, int i, int *j)
{
	char	*tmp_file_path;
	int		status;
	int		fd;

	fd = -1;
	if (is_redir_pair(cur_t) && is_s_eq(cur_t->str, "<<", true))
	{
		tmp_file_path = get_tmp_file_path(i, j);
		open_outfile(tmp_file_path, &fd);
		redirect(&fd, STDOUT_FILENO, &cur_p->redirect_fds);
		status = here_doc(ft_strdup(cur_t->next->str), STDOUT_FILENO);
		cur_p->heredoc_tmp_paths[*j] = tmp_file_path;
		reset_redirection(&cur_p->redirect_fds);
		cur_t = cur_t->next;
		*j += 1;
		if (g_vars.interrupted)
		{
			g_vars.interrupted = 0;
			access_exit_status(true, 128 + SIGINT);
			return (-1);
		}
	}
	return (0);
}

int	process_heredoc(t_proc_unit *proc_list)
{
	t_proc_unit	*cur_p;
	t_token		*cur_t;
	int			i;
	int			j;

	i = 0;
	cur_p = proc_list;
	while (cur_p)
	{
		j = 0;
		cur_t = cur_p->args;
		cur_p->heredoc_tmp_paths = malloc(sizeof(char *)
				* (count_heredoc(cur_p->args) + 1));
		while (cur_t && cur_t->next)
		{
			if (inner_process(cur_p, cur_t, i, &j) == -1)
				return (-1);
			cur_t = cur_t->next;
		}
		cur_p->heredoc_tmp_paths[j] = NULL;
		cur_p = cur_p->next;
		i++;
	}
	return (0);
}

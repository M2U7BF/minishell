/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:20:37 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/17 12:31:59 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_delim(char **delim, bool is_delim_quoted)
{
	if (is_delim_quoted)
		*delim = str_quote_removal(*delim);
}

static void	inner_process(t_proc_unit *cur_p, t_token *cur_t, int i, int *j)
{
	static char	*pathname_arr[] = {TMP_PATH, "", "_", "", NULL};
	char		*tmp_file_path;
	int			status;
	int			fd;

	while (cur_t && cur_t->next)
	{
		fd = -1;
		if (is_redir_pair(cur_t) && is_s_eq(cur_t->str, "<<", true))
		{
			pathname_arr[1] = ft_itoa(i);
			pathname_arr[3] = ft_itoa(*j);
			tmp_file_path = ft_strjoin_all(pathname_arr);
			ft_free((void **)&pathname_arr[1]);
			ft_free((void **)&pathname_arr[3]);
			open_outfile(tmp_file_path, &fd);
			redirect(&fd, STDOUT_FILENO, &cur_p->redirect_fds);
			status = here_doc(ft_strdup(cur_t->next->str), STDOUT_FILENO);
			cur_p->heredoc_tmp_paths[*j] = tmp_file_path;
			reset_redirection(&cur_p->redirect_fds);
			cur_t = cur_t->next;
			*j += 1;
		}
		cur_t = cur_t->next;
	}
}

void	process_heredoc(t_proc_unit *proc_list)
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
		inner_process(cur_p, cur_t, i, &j);
		cur_p->heredoc_tmp_paths[j] = NULL;
		cur_p = cur_p->next;
		i++;
	}
}

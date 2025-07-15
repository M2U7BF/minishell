/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:14:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 10:32:56 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_builtin(int status, t_i_mode_vars *i_vars, t_proc_unit *proc)
{
	if (status != 0)
	{
		destroy_i_vars(i_vars);
		free_env_list(&g_vars.env_list);
		g_vars.exit_status = EXIT_FAILURE;
		return (status);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		libc_error();
	if (proc->next && proc->next->read_fd != STDIN_FILENO)
	{
		if (close(proc->next->read_fd) == -1)
			libc_error();
	}
	if (is_s_eq(proc->argv[0], "exit", 1))
		ft_putendl_fd("exit", STDERR_FILENO);
	status = handle_builtin_cmd(proc->argv);
	g_vars.exit_status = status;
	return (status);
}

int	count_heredoc(t_token *token_list)
{
	t_token	*current;
	int		heredoc_count;

	heredoc_count = 0;
	current = token_list;
	while (current)
	{
		if (current->type == REDIR && ft_strncmp(current->str, "<<", 3) == 0)
			heredoc_count++;
		current = current->next;
	}
	return (heredoc_count);
}

void	finish_exec(t_i_mode_vars *i_vars, t_proc_unit *proc_list)
{
	if (i_vars->child_pids != NULL && proc_list->type != ONLY_PARENT)
		wait_child_processes(i_vars->child_pids, i_vars->pro_count);
	free_proc_list(&proc_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:14:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 08:45:34 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_builtin(int status, t_i_mode_vars *i_vars, t_proc_unit *proc)
{
	if (status != 0)
	{
		destroy_i_vars(i_vars);
		free_env_list(&g_vars.env_list);
		access_exit_status(true, EXIT_FAILURE);
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
	access_exit_status(true, status);
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

static void	wait_child_processes(int *child_pids, int pro_count)
{
	int	status;
	int	i;

	i = -1;
	status = access_exit_status(false, 0);
	while (++i < pro_count)
	{
		if (waitpid(child_pids[i], &status, 0) == -1)
			perror("waitpid");
	}
	if (WIFEXITED(status))
		access_exit_status(true, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			access_exit_status(true, 128 + SIGQUIT);
		}
		else if (WTERMSIG(status) == SIGINT)
			access_exit_status(true, 128 + SIGINT);
	}
	else
		put_error_exit("waitpid", EXIT_FAILURE);
}

void	finish_exec(t_i_mode_vars *i_vars, t_proc_unit *proc_list)
{
	if (i_vars->child_pids != NULL && proc_list->type != ONLY_PARENT)
		wait_child_processes(i_vars->child_pids, i_vars->pro_count);
	free_proc_list(&proc_list);
}

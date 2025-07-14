/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/14 11:28:28 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_list	*exec_redirection(int *status, t_proc_unit *current_proc)
{
	t_list	*redirect_fds;
	int		pipe_fds[2];

	redirect_fds = NULL;
	if (current_proc->next && current_proc->next->type == PLINE)
	{
		if (pipe(pipe_fds) == -1)
			libc_error();
		current_proc->write_fd = pipe_fds[1];
		current_proc->next->read_fd = pipe_fds[0];
	}
	redirect_fds = pipe_redirect(current_proc, redirect_fds);
	*status = open_and_redirect_files(current_proc->args, &redirect_fds,
			count_heredoc(current_proc->args));
	return (redirect_fds);
}

static void	exec_child_proc(int status, t_i_mode_vars *i_vars,
		t_proc_unit *proc_list, t_proc_unit *proc)
{
	if (proc->status != -1)
		exit(proc->status);
	if (status != 0)
	{
		destroy_i_vars(i_vars);
		free_proc_list(&proc_list);
		free_env_list(&g_vars.env_list);
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		libc_error();
	if (proc->next && proc->next->read_fd != STDIN_FILENO)
	{
		if (close(proc->next->read_fd) == -1)
			libc_error();
	}
	if (is_builtin(proc->argv[0]))
		exit(handle_builtin_cmd(proc->argv));
	execve(proc->argv[0], proc->argv,
		convert_env_list_to_array(g_vars.env_list));
	perror("execve");
	exit(EXIT_CMD_NOT_FOUND);
}

static void	exec_parent_proc(t_list **redirect_fds)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		libc_error();
	reset_redirection(*redirect_fds);
	*redirect_fds = NULL;
}

static void	wait_child_processes(int *child_pids, int pro_count)
{
	int	status;
	int	i;

	i = -1;
	status = g_vars.exit_status;
	while (++i < pro_count)
	{
		if (waitpid(child_pids[i], &status, 0) == -1)
			perror("waitpid");
	}
	if (WIFEXITED(status))
		g_vars.exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			g_vars.exit_status = 128 + SIGQUIT;
		}
		else if (WTERMSIG(status) == SIGINT)
			g_vars.exit_status = 128 + SIGINT;
	}
	else
		put_error_exit("waitpid", EXIT_FAILURE);
}

void	exec(t_i_mode_vars *i_vars, t_proc_unit *proc_list, int status)
{
	int			i;
	t_proc_unit	*current;
	t_list		*redirect_fds;

	current = proc_list;
	i = -1;
	while (proc_list && ++i < i_vars->pro_count)
	{
		redirect_fds = exec_redirection(&status, current);
		if (current->type == ONLY_PARENT)
			exec_builtin(status, i_vars, current);
		else
		{
			i_vars->child_pids[i] = fork();
			if (i_vars->child_pids[i] == 0)
				exec_child_proc(status, i_vars, proc_list, current);
			else if (i_vars->child_pids[i] == -1)
				libc_error();
		}
		exec_parent_proc(&redirect_fds);
		current = current->next;
	}
	if (i_vars->child_pids != NULL && proc_list->type != ONLY_PARENT)
		wait_child_processes(i_vars->child_pids, i_vars->pro_count);
	free_proc_list(&proc_list);
}

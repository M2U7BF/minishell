/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/23 16:39:14 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	exec_redirection(int *status, t_proc_unit *cur_proc)
{
	int	pipe_fds[2];

	if (cur_proc->next && cur_proc->next->type == PLINE)
	{
		if (pipe(pipe_fds) == -1)
			libc_error();
		cur_proc->write_fd = pipe_fds[1];
		cur_proc->next->read_fd = pipe_fds[0];
	}
	pipe_redirect(cur_proc);
	if (cur_proc->argv && !is_builtin(cur_proc->argv[0]))
	{
		access_exit_status(true, get_command_path(&cur_proc->argv[0]));
		if (access_exit_status(false, 0) != 0)
		{
			handle_error(access_exit_status(false, 0), cur_proc->argv[0]);
			*status = access_exit_status(false, 0);
			cur_proc->status = access_exit_status(false, 0);
			return ;
		}
	}
	*status = open_and_redirect_files(cur_proc->args, cur_proc);
}

static void	handle_before_exec_error(int status, t_i_mode_vars *i_vars,
		t_proc_unit *proc_list, t_proc_unit *proc)
{
	t_list	*env_list;

	env_list = access_env_list(false, NULL);
	if (proc->status != -1)
		exit(proc->status);
	if (status != 0)
	{
		destroy_i_vars(i_vars);
		reset_redirection(&proc->redirect_fds);
		free_proc_list(&proc_list);
		free_env_list(&env_list);
		exit(EXIT_FAILURE);
	}
}

static void	exec_child_proc(int status, t_i_mode_vars *i_vars,
		t_proc_unit *proc_list, t_proc_unit *proc)
{
	set_signal_handlers(true);
	handle_before_exec_error(status, i_vars, proc_list, proc);
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
		convert_env_list_to_array(access_env_list(false, NULL)));
	perror("execve");
	exit(EXIT_CMD_NOT_FOUND);
}

static void	exec_parent_proc(t_list **redirect_fds)
{
	set_signal_handlers(true);
	reset_redirection(redirect_fds);
}

void	exec(t_i_mode_vars *i_vars, t_proc_unit *proc_list, int status)
{
	int			i;
	t_proc_unit	*current;

	current = proc_list;
	i = -1;
	while (proc_list && ++i < i_vars->pro_count)
	{
		exec_redirection(&status, current);
		if (status > 128)
			return ;
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
		exec_parent_proc(&current->redirect_fds);
		current = current->next;
	}
	finish_exec(i_vars, proc_list);
}

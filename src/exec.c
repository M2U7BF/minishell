/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/07 16:34:04 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_list	*exec_redirection(int *status, t_proc_unit *current_proc)
{
	t_list	*redirect_fds;
	int		pipe_fds[2];

	redirect_fds = NULL;
	if (current_proc->next && current_proc->next->type == PIPE_LINE)
	{
		if (pipe(pipe_fds) == -1)
			libc_error();
		current_proc->write_fd = pipe_fds[1];
		current_proc->next->read_fd = pipe_fds[0];
	}
	redirect_fds = pipe_redirect(current_proc, redirect_fds);
	*status = open_and_redirect_files(current_proc->args, &redirect_fds);
	return (redirect_fds);
}

static void	update_proc(t_i_mode_vars *i_vars, t_proc_unit *proc_list)
{
	i_vars->pro_count = proc_len(proc_list);
	i_vars->child_pids = malloc(sizeof(pid_t) * i_vars->pro_count);
	if (!i_vars->child_pids)
	{
		free_proc_list(&proc_list);
		libc_error();
	}
}

static void	exec_child_proc(int status, t_i_mode_vars *i_vars,
		t_proc_unit *proc_list, t_proc_unit *proc)
{
	char	**envp_array;

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
	set_argv(proc);
	if (is_builtin(proc->argv[0]))
		exit(exec_builtin(proc->argv));
	envp_array = convert_env_list_to_array(g_vars.env_list);
	execve(proc->argv[0], proc->argv, envp_array);
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

int	exec(t_i_mode_vars *i_vars)
{
	int			i;
	t_proc_unit	*proc_list;
	t_proc_unit	*current;
	t_list		*redirect_fds;
	int			status;
	char		**trimmed_argv;

	proc_list = process_division(i_vars->token_list);
	update_proc(i_vars, proc_list);
	// printf("process_division:\n");
	// debug_put_proc_list(proc_list);
	current = proc_list;
	//------------------------------------------atashiro
	if (i_vars->pro_count == 1)
	{
		current->argv = tokens_to_arr(proc_list->args);
		// パイプがなく、コマンドが "cd" または "exit" の場合
		if (proc_list->type == CMD && current->argv
			&& is_builtin(current->argv[0]))
		{
			redirect_fds = NULL;
			i_vars->child_pids[0] = -1;
			// TODO ファイルopen時のstatus受け取り
			redirect_fds = exec_redirection(&status, current);
			trimmed_argv = trim_redirection(&current->argv);
			// argvはここで消費される
			status = exec_builtin(trimmed_argv); // 親プロセスで実行
			g_vars.exit_status = status;
			//追加？-------------------------------要検討
			free_str_array(&trimmed_argv);
			reset_redirection(redirect_fds);
			free_proc_list(&proc_list);
			// 注意: builtin_exitはプロセスを終了するので、ここに戻らない可能性がある
			return (status);
		}
		free_str_array(&current->argv);
	}
	// ------------------------------------------------------------------
	i = -1;
	while (proc_list && ++i < i_vars->pro_count)
	{
		redirect_fds = exec_redirection(&status, current);
		i_vars->child_pids[i] = fork();
		if (i_vars->child_pids[i] == 0)
			exec_child_proc(status, i_vars, proc_list, current);
		else if (i_vars->child_pids[i] == -1)
			libc_error();
		else
			exec_parent_proc(&redirect_fds);
		current = current->next;
	}
	free_proc_list(&proc_list);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 17:00:25 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_pipe(t_proc_unit *proc)
{
	if (proc->read_fd != STDIN_FILENO)
	{
		if (close(proc->read_fd) == -1)
			libc_error();
	}
	if (proc->write_fd != STDOUT_FILENO)
	{
		if (close(proc->write_fd) == -1)
			libc_error();
	}
}

void	handle_error(int status, char *cmd_path)
{
	if (status == EXIT_CMD_NOT_FOUND)
		ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd_path);
	else if (status == ENOENT)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd_path,
			strerror(status));
		g_vars.exit_status = EXIT_CMD_NOT_FOUND;
	}
	else if (status == EACCES || status == EISDIR)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd_path,
			strerror(status));
		g_vars.exit_status = EXIT_PERMISSION_DENIED;
	}
	else
		perror("minishell");
}

void exec_redirection()
{
  // 
}

int	exec(t_i_mode_vars *i_vars)
{
	int			i;
	t_proc_unit	*proc_list;
	t_proc_unit	*current_proc;
	char		**argv;
	t_list		*redirect_fds;
	int			pipe_fds[2];
	int			status;

	proc_list = process_division(i_vars->token_list);
	i_vars->pro_count = proc_len(proc_list);
	i_vars->child_pids = malloc(sizeof(pid_t) * i_vars->pro_count);
	if (!i_vars->child_pids)
	{
		free_proc_list(proc_list);
		libc_error();
	}
	i = -1;
	current_proc = proc_list;
	while (proc_list && ++i < i_vars->pro_count)
	{
		redirect_fds = NULL;
		if (current_proc->next && current_proc->next->type == PIPE_LINE)
		{
			if (pipe(pipe_fds) == -1)
				libc_error();
			current_proc->write_fd = pipe_fds[1];
			current_proc->next->read_fd = pipe_fds[0];
		}
		redirect_fds = pipe_redirect(current_proc, redirect_fds);
		status = open_and_redirect_files(current_proc, &redirect_fds);
		i_vars->child_pids[i] = fork();
		if (i_vars->child_pids[i] == 0)
		{
			if (status != 0)
			{
				destroy_i_vars(i_vars);
				free_proc_list(proc_list);
				exit(EXIT_FAILURE);
			}
			if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
				libc_error();
			if (current_proc->next
				&& current_proc->next->read_fd != STDIN_FILENO)
			{
				if (close(current_proc->next->read_fd) == -1)
					libc_error();
			}
			argv = tokens_to_arr(current_proc->args);
			argv = trim_redirection(&argv);
			if (!argv)
				exit(EXIT_SUCCESS);
			g_vars.exit_status = get_command_path(&argv[0]);
			if (g_vars.exit_status != 0)
			{
				handle_error(g_vars.exit_status, argv[0]);
				exit(g_vars.exit_status);
			}
			execve(argv[0], argv, __environ);
			perror("execve");
			exit(EXIT_CMD_NOT_FOUND);
		}
		else if (i_vars->child_pids[i] == -1)
			libc_error();
		else
		{
			if (signal(SIGINT, SIG_IGN) == SIG_ERR)
				libc_error();
			reset_redirection(redirect_fds);
		}
		current_proc = current_proc->next;
	}
	free_proc_list(proc_list);
	return (0);
}

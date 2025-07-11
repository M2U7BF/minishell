/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:14:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/10 15:14:09 by kkamei           ###   ########.fr       */
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
	status = handle_builtin_cmd(proc->argv);
	g_vars.exit_status = status;
	return (status);
}

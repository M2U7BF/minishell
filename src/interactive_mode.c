/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/07 17:53:40 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_runtime_data	g_vars = {};

void	wait_child_processes(int *child_pids, int pro_count)
{
	int	status;
	int	i;

	i = -1;
	status = g_vars.exit_status;
	while (++i < pro_count)
	{
		if (child_pids[i] == -1)
			return ;
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

int	exec_interactive(t_i_mode_vars *i_vars)
{
	rl_outstream = stderr;
	while (1)
	{
		if (i_vars->input)
			ft_free((void **)&i_vars->input);
		set_signal_handlers();
		i_vars->input = readline("minishell$ ");
		if (!i_vars->input)
			exit(g_vars.exit_status);
		else if (i_vars->input[0] == '\0')
			continue ;
		add_history(i_vars->input);
		ft_strtrim_front(&i_vars->input, DEFAULT_BLANK);
		i_vars->token_list = tokenize(i_vars->input);
		if (!i_vars->token_list)
			continue ;
		parse(i_vars);
		exec(i_vars);
		if (i_vars->child_pids != NULL)
			wait_child_processes(i_vars->child_pids, i_vars->pro_count);
		destroy_i_vars(i_vars);
		free_env_list(&g_vars.env_list);
	}
	return (0);
}

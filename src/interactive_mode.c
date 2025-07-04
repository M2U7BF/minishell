/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/04 11:34:39 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_runtime_data	g_vars = {};

void	wait_child_processes(int *child_pids, int pro_count)
{
	int	status;
	int	i;

	i = 0;
	while (i < pro_count)
	{
		if (waitpid(child_pids[i], &status, 0) == -1)
			perror("waitpid");
		i++;
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

int	exec_interactive(t_i_mode_vars *i_vars, t_exec_vars *e_vars)
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
		if (is_quotation_error(i_vars->input) != 0)
			exit(EXIT_FAILURE);
		ft_strtrim_front(&i_vars->input, DEFAULT_BLANK);
		i_vars->token_list = tokenize(i_vars->input);
		if (!i_vars->token_list)
			continue ;
		parse(i_vars, e_vars->env_list);
		quote_removal(i_vars->token_list);
		exec(i_vars, e_vars->env_list);
		if (i_vars->child_pids != NULL)
			wait_child_processes(i_vars->child_pids, i_vars->pro_count);
		destroy_i_vars(i_vars);\
	}
	return (0);
}

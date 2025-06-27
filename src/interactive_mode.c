/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 16:15:08 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_runtime_data	g_runtime_data = {};

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
		g_runtime_data.exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			g_runtime_data.exit_status = 128 + SIGQUIT;
		}
		else if (WTERMSIG(status) == SIGINT)
			g_runtime_data.exit_status = 128 + SIGINT;
	}
	else
		put_error_exit("waitpid", EXIT_FAILURE);
}

int	exec_interactive(t_exec_vars *e_vars)
{
	t_i_mode_vars	*i_vars;

	init_i_vars(&e_vars->i_vars);
	i_vars = &e_vars->i_vars;
	rl_outstream = stderr;
	g_runtime_data.signal = 0;
	g_runtime_data.exit_status = EXIT_SUCCESS;
	while (1)
	{
		handle_signal();
		g_runtime_data.signal = 0;
		i_vars->input_line = readline(i_vars->prompt);
		if (!i_vars->input_line)
		{
			if (g_runtime_data.signal == SIGINT)
				g_runtime_data.exit_status = 130;
			exit(g_runtime_data.exit_status);
		}
		if (i_vars->input_line[0] != '\0')
			add_history(i_vars->input_line);
		else
		{
			ft_free(i_vars->input_line);
			continue ;
		}
		i_vars->input_line = ft_strtrim_front(i_vars->input_line,
				DEFAULT_BLANK);
		if (is_quotation_error(i_vars->input_line) != 0)
			exit(EXIT_FAILURE);
		i_vars->token_list = tokenize(i_vars->input_line);
		if (!i_vars->token_list)
		{
			ft_free(i_vars->input_line);
			continue ;
		}
		if (is_syntax_error(i_vars->token_list))
		{
			ft_dprintf(STDERR_FILENO, "syntax_error\n");
			destroy_i_vars(i_vars);
			exit(EXIT_SYNTAX_ERROR);
		}
		parse(i_vars);
		quote_removal(i_vars->token_list);
		exec(i_vars);
		wait_child_processes(i_vars->child_pids, i_vars->pro_count);
		destroy_i_vars(i_vars);
		i_vars->child_pids = NULL;
	}
	clear_history();
	return (0);
}

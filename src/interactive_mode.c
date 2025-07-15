/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 11:45:57 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_runtime_data	g_vars = {};

static void	process_ctrl_d(void)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	free_env_list(&g_vars.env_list);
	rl_clear_history();
	exit(g_vars.exit_status);
}

int	exec_interactive(t_i_mode_vars *i_vars)
{
	char	*prompt;

	rl_outstream = stderr;
	while (1)
	{
		g_vars.interrupted = 0;
		if (i_vars->input)
			ft_free((void **)&i_vars->input);
		set_signal_handlers(false);
		prompt = get_prompt();
		i_vars->input = readline(prompt);
		ft_free((void **)&prompt);
		if (g_vars.interrupted)
			continue ;
		if (!i_vars->input)
			process_ctrl_d();
		else if (i_vars->input[0] == '\0')
			continue ;
		add_history(i_vars->input);
		ft_strtrim_front(&i_vars->input, DEFAULT_BLANK);
		i_vars->token_list = tokenize(i_vars->input);
		if (!i_vars->token_list)
			continue ;
		if (parse(i_vars) == -1)
			continue ;
		exec(i_vars, process_division(i_vars), EXIT_SUCCESS);
		destroy_i_vars(i_vars);
	}
	return (0);
}

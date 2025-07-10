/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/09 17:03:31 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_runtime_data	g_vars = {};

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
		destroy_i_vars(i_vars);
		free_env_list(&g_vars.env_list);
	}
	return (0);
}

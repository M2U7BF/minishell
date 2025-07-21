/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_noninteractive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:50:06 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 15:23:24 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_input_value(t_i_mode_vars *i_vars)
{
	ft_free((void **)&i_vars->prompt);
	if (g_vars.interrupted)
		return (-1);
	if (!i_vars->input)
		process_ctrl_d();
	else if (i_vars->input[0] == '\n')
		return (-1);
	else if (i_vars->input[0] == '\0')
		return (-1);
	add_history(i_vars->input);
	ft_strtrim_front(&i_vars->input, DEFAULT_BLANK);
	i_vars->token_list = tokenize(i_vars->input);
	return (0);
}

int	exec_noninteractive(t_i_mode_vars *i_vars)
{
	t_proc_unit	*proc_list;

	while (1)
	{
		g_vars.interrupted = 0;
		if (i_vars->input)
			ft_free((void **)&i_vars->input);
		set_signal_handlers(false);
		i_vars->input = get_next_line(STDIN_FILENO);
		trim_endl(&i_vars->input);
		if (handle_input_value(i_vars) == -1)
			continue ;
		if (!i_vars->token_list)
			continue ;
		if (parse(i_vars) == -1)
			continue ;
		proc_list = process_division(i_vars);
		if (process_heredoc(proc_list) == -1)
			continue ;
		exec(i_vars, proc_list, EXIT_SUCCESS);
		destroy_i_vars(i_vars);
	}
	return (0);
}

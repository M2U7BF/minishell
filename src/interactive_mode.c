/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/23 16:41:18 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_ctrl_d(void)
{
	t_list	*env_list;

	env_list = access_env_list(false, NULL);
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	free_env_list(&env_list);
	rl_clear_history();
	exit(access_exit_status(false, 0));
}

static int	handle_input_value(t_i_mode_vars *i_vars)
{
	ft_free((void **)&i_vars->prompt);
	if (g_signum == SIGINT)
		return (-1);
	if (!i_vars->input)
		process_ctrl_d();
	else if (i_vars->input[0] == '\0')
		return (-1);
	add_history(i_vars->input);
	ft_strtrim_front(&i_vars->input, DEFAULT_BLANK);
	i_vars->token_list = tokenize(i_vars->input);
	return (0);
}

int	exec_interactive(t_i_mode_vars *i_vars)
{
	t_proc_unit	*proc_list;

	rl_outstream = stderr;
	while (1)
	{
		g_signum = 0;
		i_vars->prompt = get_prompt();
		if (i_vars->input)
			ft_free((void **)&i_vars->input);
		set_signal_handlers(false);
		i_vars->input = readline(i_vars->prompt);
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

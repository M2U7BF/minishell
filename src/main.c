/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 08:45:43 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_exec_vars	e_vars;

	(void)argv;
	if (argc > 3)
		return (EXIT_FAILURE);
	init_e_vars(&e_vars);
	init_env_list(&g_vars.env_list, envp);
	if (parse_exec_arg(argc) == -1)
	{
		free_env_list(&g_vars.env_list);
		return (EXIT_FAILURE);
	}
	g_vars.signal = 0;
	access_exit_status(true, EXIT_SUCCESS);
	if (e_vars.mode == INTERACTIVE)
	{
		init_i_vars(&e_vars.i_vars);
		exec_interactive(&e_vars.i_vars);
		rl_clear_history();
	}
	free_env_list(&g_vars.env_list);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 17:03:03 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t signum = 0;

int	main(int argc, char *argv[], char **envp)
{
	t_exec_vars	e_vars;
	t_list		*env_list;

	(void)argv;
	env_list = access_env_list(false, NULL);
	if (argc > 3)
		return (EXIT_FAILURE);
	init_e_vars(&e_vars);
	init_env_list(&env_list, envp);
	if (parse_exec_arg(argc) == -1)
	{
		free_env_list(&env_list);
		return (EXIT_FAILURE);
	}
	signum = 0;
	access_exit_status(true, EXIT_SUCCESS);
	init_i_vars(&e_vars.i_vars);
	if (!isatty(STDIN_FILENO))
		exec_noninteractive(&e_vars.i_vars);
	else if (e_vars.mode == INTERACTIVE)
		exec_interactive(&e_vars.i_vars);
	rl_clear_history();
	free_env_list(&env_list);
	return (EXIT_SUCCESS);
}

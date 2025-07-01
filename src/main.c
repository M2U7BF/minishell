/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 11:04:59 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[])
{
	t_exec_vars	e_vars;

	if (argc > 3)
		return (EXIT_FAILURE);
	init_e_vars(&e_vars);
	if (parse_exec_arg(argc, argv, &e_vars) == -1)
		return (EXIT_FAILURE);
	g_runtime_data.signal = 0;
	g_runtime_data.exit_status = EXIT_SUCCESS;
	if (e_vars.mode == INTERACTIVE)
	{
		init_i_vars(&e_vars.i_vars);
		exec_interactive(&e_vars.i_vars);
		clear_history();
	}
	return (EXIT_SUCCESS);
}

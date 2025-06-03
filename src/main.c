/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/03 11:43:17 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_exec_vars	e_vars;

	(void)envp;
	if (argc > 3)
		return (EXIT_FAILURE);
	init_e_vars(&e_vars);
	if (parse_exec_arg(argc, argv, &e_vars) == -1)
		return (EXIT_FAILURE);
	if (e_vars.mode == INTERACTIVE)
		exec_interactive(&e_vars);
	// else
	// 	exec_non_interactive(&e_vars);
	return (EXIT_SUCCESS);
}

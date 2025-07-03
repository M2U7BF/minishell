/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:19:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 11:24:02 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_e_vars(t_exec_vars *e_vars)
{
	e_vars->mode = INTERACTIVE;
}

int	parse_exec_arg(int argc, char *argv[], t_exec_vars *e_vars)
{
	if (argc == 1)
		return (0);
	else if (argc == 3 && ft_strncmp(argv[1], "-c", 3) == 0)
	{
		e_vars->mode = NON_INTERACTIVE;
		e_vars->ni_vars.input_line = argv[2];
		return (0);
	}
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:19:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 12:10:55 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_e_vars(t_exec_vars *e_vars)
{
	e_vars->mode = INTERACTIVE;
}

int	parse_exec_arg(int argc)
{
	if (argc == 1)
		return (0);
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_mode_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:17:43 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 11:24:41 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_i_vars(t_i_mode_vars *i_vars)
{
	i_vars->input_line = NULL;
	i_vars->prompt = "hoge> ";
	i_vars->token_list = NULL;
	i_vars->child_pids = NULL;
}

void	destroy_i_vars(t_i_mode_vars *vars)
{
	ft_free(vars->input_line);
	free_token_list(vars->token_list);
	ft_free(vars->child_pids);
}

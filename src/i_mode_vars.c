/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_mode_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:17:43 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 15:04:39 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_i_vars(t_i_mode_vars *i_vars)
{
	i_vars->input = NULL;
	i_vars->prompt = get_prompt();
	i_vars->token_list = NULL;
	i_vars->child_pids = NULL;
}

void	destroy_i_vars(t_i_mode_vars *vars)
{
	ft_free((void **)&vars->input);
	// ft_free((void **)&vars->prompt);
	free_token_list(&vars->token_list);
	ft_free((void **)&vars->child_pids);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_mode_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:17:43 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/03 10:48:47 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void init_i_vars(t_i_mode_vars *i_vars)
{
  i_vars->input_line = NULL;
	// TODO プロンプトはPS1に合わせて生成が必要？
  i_vars->prompt = "hoge> ";
}

void	destroy_i_vars(t_i_mode_vars *vars)
{
	ft_free(vars->input_line);
	ft_free(vars->prompt);
	ft_free(vars);
}

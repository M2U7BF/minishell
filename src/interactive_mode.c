/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/05 11:32:16 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_interactive(t_exec_vars *e_vars)
{
	t_i_mode_vars	*i_vars;

	handle_signal();
	init_i_vars(&e_vars->i_vars);
	i_vars = &e_vars->i_vars;
	rl_outstream = stderr;
	while (1)
	{
		i_vars->input_line = readline(i_vars->prompt);
		if (!i_vars->input_line)
			exit(EXIT_SUCCESS);
		if (i_vars->input_line[0] != '\0')
			add_history(i_vars->input_line);
		// パース
		parse(i_vars);
		put_strarr(i_vars->words);
		// コマンド実行
		ft_free(i_vars->input_line);
	}
}

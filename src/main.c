/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/03 11:24:52 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char	*redirect_expansion(char *s)
// {
// 	//
// }

// char	*pipe_expansion(char *s)
// {
// 	//
// }

// char	*variable_expansion(char *s)
// {
// 	// $を探す
// 	// 変数と一致するか検索する
// 	// 一致し次第、置換し、文字列を更新する
// }

// char	*parse(t_i_mode_vars *vars)
// {
// 	variable_expansion(vars->input_line);
// 	// TODO プロセス数を判定後、下記を実施。
// 	redirect_expansion(vars->input_line);
// 	pipe_expansion(vars->input_line);
// }

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

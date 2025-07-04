/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/04 11:54:40 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// main関数に `char **envp` を追加
int	main(int argc, char *argv[], char **envp)
{
	t_exec_vars	e_vars;

	if (argc > 3)
		return (EXIT_FAILURE);
	init_e_vars(&e_vars);
	init_env_list(&g_vars.env_list, envp); // <-- 環境変数リストを初期化
	if (parse_exec_arg(argc, argv, &e_vars) == -1)
	{
		free_env_list(&g_vars.env_list); // <-- 失敗時も解放
		return (EXIT_FAILURE);
	}
	g_vars.signal = 0;
	g_vars.exit_status = EXIT_SUCCESS;
	if (e_vars.mode == INTERACTIVE)
	{
		init_i_vars(&e_vars.i_vars);
		exec_interactive(&e_vars.i_vars);
		clear_history();
	}
	free_env_list(&g_vars.env_list);
	return (EXIT_SUCCESS);
}

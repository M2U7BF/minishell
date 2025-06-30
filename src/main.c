/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/30 12:23:42 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include "../include/minishell.h"

// main関数に `char **envp` を追加
int	main(int argc, char *argv[], char **envp)
{
	t_exec_vars	e_vars;

	if (argc > 3)
		return (EXIT_FAILURE);
	init_e_vars(&e_vars);
	init_env_list(&e_vars.env_list, envp); // <-- 環境変数リストを初期化

	if (parse_exec_arg(argc, argv, &e_vars) == -1)
	{
		free_env_list(&e_vars.env_list); // <-- 失敗時も解放
		return (EXIT_FAILURE);
	}

	if (e_vars.mode == INTERACTIVE)
		exec_interactive(&e_vars);
	// else
	// 	exec_non_interactive(&e_vars);
	free_env_list(&e_vars.env_list); // <-- 正常終了時に解放
	return (EXIT_SUCCESS);
}

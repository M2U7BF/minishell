/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/02 17:45:56 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *redirect_expansion(char *s)
{
  // 
}

char *pipe_expansion(char *s)
{
  // 
}

char *variable_expansion(char *s)
{
	// $を探す
	// 変数と一致するか検索する
	// 一致し次第、置換し、文字列を更新する
}

char *parse(t_vars	*vars)
{
  variable_expansion(vars->input_line);
	// TODO プロセス数を判定後、下記を実施。
	redirect_expansion(vars->input_line);
	pipe_expansion(vars->input_line);
}

t_vars	*init_vars(void)
{
	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	vars->input_line = NULL;
	// TODO プロンプトはPS1に合わせて生成が必要？
	vars->prompt = "hoge> ";
	return (vars);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_vars	*vars;

	(void)argv;
	(void)envp;
	if (argc != 1)
		return (EXIT_FAILURE);
	handle_signal();
	vars = init_vars();
	while (1)
	{
		vars->input_line = readline(vars->prompt);
		printf("s:%s\n", vars->input_line);
		if (!vars->input_line)
			exit(EXIT_SUCCESS);
		// 履歴への追加
		add_history(vars->input_line);
		// パース
    parse(vars);
		// コマンド実行
		ft_free(vars->input_line);
	}
	return (EXIT_SUCCESS);
}

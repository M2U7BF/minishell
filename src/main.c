/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:39:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/02 12:55:41 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_free(void *p)
{
  free(p);
  p = NULL;
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
		return (1);
	vars = init_vars();
  while (1)
  {
    vars->input_line = readline(vars->prompt);
    printf("s:%s\n", vars->input_line);
    // パース
    // コマンド実行
    ft_free(vars->input_line);
  }
	return (0);
}

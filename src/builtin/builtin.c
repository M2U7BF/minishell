/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:09:08 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/04 11:46:53 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 5) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (builtin_echo(argv));
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (builtin_cd(argv)); // 引数追加
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		return (builtin_export(argv)); // 引数追加
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (builtin_unset(argv)); // 引数追加
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (builtin_env()); // 引数追加
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (builtin_exit(argv));
	return (127);
}

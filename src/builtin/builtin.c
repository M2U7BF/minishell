/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:09:08 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/20 16:38:49 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0 || ft_strncmp(cmd, "cd", 2) == 0
		|| ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "export", 6) == 0
		|| ft_strncmp(cmd, "unset", 5) == 0 || ft_strncmp(cmd, "env", 3) == 0
		|| ft_strncmp(cmd, "exit", 4) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **argv)
{
	if (ft_strncmp(argv[0], "echo", 4) == 0)
		builtin_echo(argv);
	// else if (ft_strncmp(argv[0], "cd", 2) == 0)
	// 	builtin_cd(argv);
	else if (ft_strncmp(argv[0], "pwd", 3) == 0)
		builtin_pwd();
	// else if (ft_strncmp(argv[0], "export", 6) == 0)
	// 	builtin_export(argv);
	// else if (ft_strncmp(argv[0], "unset", 5) == 0)
	// 	builtin_unset(argv);
	else if (ft_strncmp(argv[0], "env", 3) == 0)
		builtin_env();
	else if (ft_strncmp(argv[0], "exit", 4) == 0)
		builtin_exit(argv);
	else
		return (0);
	return (1);
}


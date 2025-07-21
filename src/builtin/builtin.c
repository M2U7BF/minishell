/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:09:08 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 08:46:11 by kkamei           ###   ########.fr       */
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
	if (is_variable_assignment(cmd))
		return (1);
	return (0);
}

static int	exec_builtin_command(char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (builtin_echo(argv));
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (builtin_cd(argv));
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		return (builtin_export(argv));
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (builtin_unset(argv));
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (builtin_env());
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (builtin_exit(argv));
	return (-1);
}

static int	handle_variable_assignment_command(char **argv)
{
	if (is_variable_assignment(argv[0]))
	{
		if (argv[1] != NULL)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n",
				argv[1]);
			return (127);
		}
		handle_variable_assignment(argv[0]);
		return (access_exit_status(false, 0));
	}
	return (-1);
}

int	handle_builtin_cmd(char **argv)
{
	int	ret;

	if (!argv || !argv[0])
		return (127);
	ret = exec_builtin_command(argv);
	if (ret != -1)
		return (ret);
	ret = handle_variable_assignment_command(argv);
	if (ret != -1)
		return (ret);
	return (127);
}

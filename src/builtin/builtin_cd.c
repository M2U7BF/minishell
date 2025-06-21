/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:44 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/21 19:53:57 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// cd 土日にやらる

#include "../../include/minishell.h"
#include <unistd.h>      // chdir, getcwd
#include <stdlib.h>      // getenv, setenv
#include <stdio.h>       // perror
#include <string.h>      // strerror
#include <errno.h>       // errno
#include <limits.h>      // PATH_MAX

static int	update_env_vars(void);
static int	change_dir_and_update_env(const char *path);
static int	update_env_vars(void)
{
	char	cwd[1000];
	char	*old_pwd;

	old_pwd = getenv("PWD");
	if (old_pwd != NULL)
	{
		if (setenv("OLDPWD", old_pwd, 1) != 0)
		{
			perror("minishell: cd");
			return (1);
		}
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd");
		return (1);
	}
	if (setenv("PWD", cwd, 1) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

static int	change_dir_and_update_env(const char *path)
{
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (update_env_vars());
}

/*
  コマンドの終了ステータス。成功で0、失敗で1。
 */
int	builtin_cd(char **argv)
{
	char	*path;

	if (arrlen(argv) > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (arrlen(argv) == 1 || ft_strncmp(argv[1], "~", 2) == 0)
	{
		path = getenv("HOME");
		if (path == NULL || *path == '\0')
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = getenv("OLDPWD");
		if (path == NULL)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
			return (1);
		}
		ft_putendl_fd(path, STDOUT_FILENO);
	}
	else
	{
		path = argv[1];
	}
	return (change_dir_and_update_env(path));
}

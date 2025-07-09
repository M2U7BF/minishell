/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:44 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/09 14:52:32 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	update_env_vars(t_list *env_list)
{
	char	cwd[1024];
	char	*old_pwd;

	old_pwd = get_env_value(env_list, "PWD");
	if (old_pwd != NULL)
	{
		set_env_var(&env_list, "OLDPWD", old_pwd);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd");
		return (1);
	}
	set_env_var(&env_list, "PWD", cwd);
	return (0);
}

static int	change_dir_and_update_env(const char *path, t_list *env_list)
{
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path,
			strerror(errno));
		return (1);
	}
	return (update_env_vars(env_list));
}

int	handle_cd_home(void)
{
	char	*path;

	path = get_env_value(g_vars.env_list, "HOME");
	if (path == NULL || *path == '\0')
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
		return (1);
	}
	return (change_dir_and_update_env(path, g_vars.env_list));
}

int	handle_cd_dash(void)
{
	char	*path;

	path = get_env_value(g_vars.env_list, "OLDPWD");
	if (path == NULL)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	return (change_dir_and_update_env(path, g_vars.env_list));
}

int	builtin_cd(char **argv)
{
	int	argc;

	argc = arrlen(argv);
	if (argc > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (argc == 1 || (argc > 1 && ft_strncmp(argv[1], "~", 2) == 0))
		return (handle_cd_home());
	else if (argc > 1 && ft_strncmp(argv[1], "-", 2) == 0)
		return (handle_cd_dash());
	return (change_dir_and_update_env(argv[1], g_vars.env_list));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:57:51 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/09 17:10:34 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_env_vars(t_list *env_list)
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

int	change_dir_and_update_env(const char *path, t_list *env_list)
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

int	handle_cd_dotdot(void)
{
	char	*pwd;
	char	*parent_path;
	char	*last_slash;
	int		ret;

	pwd = get_env_value(g_vars.env_list, "PWD");
	if (pwd == NULL)
		return (change_dir_and_update_env("..", g_vars.env_list));
	last_slash = ft_strrchr(pwd, '/');
	if (last_slash == pwd)
		parent_path = ft_strdup("/");
	else if (last_slash != NULL)
		parent_path = ft_substr(pwd, 0, last_slash - pwd);
	else
		return (change_dir_and_update_env("..", g_vars.env_list));
	if (parent_path == NULL)
		return (1);
	ret = change_dir_and_update_env(parent_path, g_vars.env_list);
	free(parent_path);
	return (ret);
}

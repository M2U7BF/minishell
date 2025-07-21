/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:57:51 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 16:51:28 by kkamei           ###   ########.fr       */
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

int	handle_cd_home(void)
{
	char	*path;

	path = get_env_value(access_env_list(false, NULL), "HOME");
	if (path == NULL || *path == '\0')
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
		return (1);
	}
	return (change_dir_and_update_env(path, access_env_list(false, NULL)));
}

int	handle_cd_dash(void)
{
	char	*path;

	path = get_env_value(access_env_list(false, NULL), "OLDPWD");
	if (path == NULL)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	return (change_dir_and_update_env(path, access_env_list(false, NULL)));
}

int	handle_cd_dotdot(void)
{
	char	*pwd;
	char	*parent_path;
	char	*last_slash;
	int		ret;

	pwd = get_env_value(access_env_list(false, NULL), "PWD");
	if (pwd == NULL)
		return (change_dir_and_update_env("..", access_env_list(false, NULL)));
	last_slash = ft_strrchr(pwd, '/');
	if (last_slash == pwd)
		parent_path = ft_strdup("/");
	else if (last_slash != NULL)
		parent_path = ft_substr(pwd, 0, last_slash - pwd);
	else
		return (change_dir_and_update_env("..", access_env_list(false, NULL)));
	if (parent_path == NULL)
		return (1);
	ret = change_dir_and_update_env(parent_path, access_env_list(false, NULL));
	free(parent_path);
	return (ret);
}

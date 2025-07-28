/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:47:24 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/28 09:42:41 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	change_directory(const char *path)
{
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}

static void	update_pwd_env_vars(const char *path, t_list *env_list)
{
	char	*old_pwd;
	char	*new_pwd;
	char	cwd_buf[1024];

	old_pwd = get_env_value(env_list, "PWD");
	if (old_pwd)
		set_env_var(&env_list, "OLDPWD", old_pwd);
	if (path[0] == '/')
		new_pwd = ft_strdup(path);
	else
		new_pwd = resolve_path(path, env_list);
	if (!new_pwd)
	{
		if (getcwd(cwd_buf, sizeof(cwd_buf)))
			set_env_var(&env_list, "PWD", cwd_buf);
		return ;
	}
	set_env_var(&env_list, "PWD", new_pwd);
	free(new_pwd);
}

int	change_dir_and_update_env(const char *path, t_list *env_list)
{
	char	*path_copy;

	path_copy = ft_strdup(path);
	if (!path_copy)
		libc_error();
	if (change_directory(path_copy) != 0)
	{
		free(path_copy);
		return (1);
	}
	update_pwd_env_vars(path_copy, env_list);
	free(path_copy);
	return (0);
}

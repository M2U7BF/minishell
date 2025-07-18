/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:22:05 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/18 10:51:50 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	search_command_path(char **cmd_name, char **path_env)
{
	int		i;
	char	*path;
	char	*tmp;

	i = 0;
	while (path_env && path_env[i])
	{
		tmp = ft_strjoin(path_env[i], "/");
		path = ft_strjoin(tmp, *cmd_name);
		ft_free((void **)&tmp);
		if (access(path, F_OK) != 0)
		{
			ft_free((void **)&path);
			i++;
			continue ;
		}
		ft_free((void **)cmd_name);
		*cmd_name = path;
		if (access(*cmd_name, X_OK) != 0)
			return (errno);
		return (0);
	}
	if (access(*cmd_name, F_OK) == 0 && access(*cmd_name, R_OK) != 0)
		return (errno);
	return (EXIT_CMD_NOT_FOUND);
}

void	search_command_without_path_env(char **cmd_name, int *status)
{
	char	*new_cmd;

	{
		if (access(*cmd_name, X_OK) == 0)
		{
			new_cmd = ft_strjoin("./", *cmd_name);
			ft_free((void **)cmd_name);
			*cmd_name = new_cmd;
			*status = 0;
		}
		else
			*status = EXIT_CMD_NOT_FOUND;
	}
}

static void	search_command_abs_path(char **cmd_name, int *status)
{
	if (access(*cmd_name, X_OK) != 0)
		*status = errno;
	else if (!is_readable_file(*cmd_name))
		*status = EISDIR;
}

int	get_command_path(char **cmd_name)
{
	char	**path_env;
	int		status;

	path_env = ft_split(get_env_value(g_vars.env_list, "PATH"), ':');
	status = 0;
	if ((*cmd_name)[0] == '\0')
		status = EXIT_CMD_NOT_FOUND;
	else if (ft_strchr((*cmd_name), '/'))
		search_command_abs_path(cmd_name, &status);
	else if (!path_env)
		search_command_without_path_env(cmd_name, &status);
	else
	{
		if (ft_strncmp(*cmd_name, ".", 2) == 0)
			status = EISDIR;
		else if (ft_strncmp(*cmd_name, "..", 3) == 0)
			status = EISDIR;
		else
			status = search_command_path(cmd_name, path_env);
	}
	free_str_array(&path_env);
	return (status);
}

void	handle_error(int status, char *cmd_path)
{
	if (status == EXIT_CMD_NOT_FOUND)
		ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd_path);
	else if (status == ENOENT)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd_path,
			strerror(status));
		g_vars.exit_status = EXIT_CMD_NOT_FOUND;
	}
	else if (status == EACCES || status == EISDIR)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd_path,
			strerror(status));
		g_vars.exit_status = EXIT_PERMISSION_DENIED;
	}
	else if (status > 128)
		g_vars.exit_status = status;
	else
		perror("minishell");
}

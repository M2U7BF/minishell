/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:44 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/19 16:07:31 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_cd(char **argv)
{
	int		argc;
	char	*path;
	char	*resolved_path;
	int		status;

	argc = arrlen(argv);
	if (argc > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (argc == 1 || (argc > 1 && ft_strncmp(argv[1], "~", 2) == 0))
		return (handle_cd_home());
	if (argc > 1 && ft_strncmp(argv[1], "-", 2) == 0)
		return (handle_cd_dash());
	path = argv[1];
	if (path[0] != '/' && ft_strnstr(path, "..", ft_strlen(path)))
	{
		resolved_path = resolve_path(path, g_vars.env_list);
		if (resolved_path)
		{
			status = change_dir_and_update_env(resolved_path, g_vars.env_list);
			free(resolved_path);
			return (status);
		}
	}
	return (change_dir_and_update_env(path, g_vars.env_list));
}

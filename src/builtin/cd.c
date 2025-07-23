/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:44 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/23 16:26:19 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_too_many_args(int argc)
{
	if (argc > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	return (0);
}

static int	handle_special_cd_cases(int argc, char **argv)
{
	if (argc == 1 || (argc > 1 && ft_strncmp(argv[1], "~", 2) == 0))
		return (handle_cd_home());
	if (argc > 1 && ft_strncmp(argv[1], "-", 2) == 0)
		return (handle_cd_dash());
	return (-1);
}

static int	handle_relative_path_cd(const char *path)
{
	char	*resolved_path;
	int		status;

	resolved_path = resolve_path(path, access_env_list(false, NULL));
	if (!resolved_path)
		return (1);
	status = change_dir_and_update_env(resolved_path, access_env_list(false,
				NULL));
	free(resolved_path);
	return (status);
}

int	builtin_cd(char **argv)
{
	int		argc;
	char	*path;
	int		status;

	argc = arrlen(argv);
	if (handle_too_many_args(argc))
		return (1);
	status = handle_special_cd_cases(argc, argv);
	if (status != -1)
		return (status);
	path = argv[1];
	if (path[0] != '/' && ft_strnstr(path, "..", ft_strlen(path)))
		return (handle_relative_path_cd(path));
	return (change_dir_and_update_env(path, access_env_list(false, NULL)));
}

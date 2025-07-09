/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:44 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/09 16:58:20 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	else if (argc > 1 && ft_strncmp(argv[1], "..", 3) == 0)
		return (handle_cd_dotdot());
	return (change_dir_and_update_env(argv[1], g_vars.env_list));
}

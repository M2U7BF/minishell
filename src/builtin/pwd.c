/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:55 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 16:53:57 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_pwd(void)
{
	char	*cwd;
	char	physical_cwd[1024];

	cwd = get_env_value(access_env_list(false, NULL), "PWD");
	if (cwd != NULL)
		ft_putendl_fd(cwd, STDOUT_FILENO);
	else
	{
		if (getcwd(physical_cwd, sizeof(physical_cwd)) != NULL)
			ft_putendl_fd(physical_cwd, STDOUT_FILENO);
		else
		{
			perror("minishell: pwd");
			return (1);
		}
	}
	return (0);
}

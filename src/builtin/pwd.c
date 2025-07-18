/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:55 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/18 12:26:50 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_pwd(void)
{
	char	*pwd;
	char	cwd[1024];

	pwd = get_env_value(g_vars.env_list, "PWD");
	if (pwd)
		ft_putendl_fd(pwd, STDOUT_FILENO);
	else if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putendl_fd(cwd, STDOUT_FILENO);
	else
	{
		perror("minishell: pwd");
		return (1);
	}
	return (0);
}

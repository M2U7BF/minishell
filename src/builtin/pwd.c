/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:55 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/20 15:49:00 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
// #include <limits.h>　//PATH_MAX

// pwd 完成？
void	builtin_pwd(void)
{
	char	cwd[1000]; //PATH_MAXを検討(もしくはgetenvPWD?())

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		ft_putendl_fd("ビルトインです", STDOUT_FILENO);
	}
	else
	{
		perror("pwd");
	}
}

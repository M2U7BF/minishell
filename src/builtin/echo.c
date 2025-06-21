/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:45:02 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/21 16:30:33 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// echo オプションのパターンをどこまでやるか?　リダイレクトに対応すべき(exec側を修正？)
void	builtin_echo(char **argv)
{
	int	n_option;
	int	i;

	n_option = 0;
	i = 1;
	if (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0) //本家と比較し、要改善
	{
		n_option = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (n_option == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
}


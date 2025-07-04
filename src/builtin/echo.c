/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:45:02 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/30 14:20:21 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// echo オプションのパターンをどこまでやるか?　リダイレクトに対応すべき(exec側を修正？)
int	builtin_echo(char **argv)
{
	int n_option;
	int i;
	int j;

	n_option = 0;
	i = 1;
	while (argv[i] && argv[i][0] == '-')
	{
		j = 1;

		while (argv[i][j] == 'n')
			j++;
		if (j > 1 && argv[i][j] == '\0')
		{
			n_option = 1;
			i++;
		}
		else
			break;
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
	return (0);
}

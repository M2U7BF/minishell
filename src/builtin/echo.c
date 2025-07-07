/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:45:02 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 11:24:50 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	parse_n_option(char **argv, int *index)
{
	int	i;
	int	j;
	int	n_option;

	n_option = 0;
	i = *index;
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
			break ;
	}
	*index = i;
	return (n_option);
}

static void	print_echo_arguments(char **argv, int start)
{
	while (argv[start])
	{
		ft_putstr_fd(argv[start], STDOUT_FILENO);
		if (argv[start + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		start++;
	}
}

int	builtin_echo(char **argv)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = parse_n_option(argv, &i);
	print_echo_arguments(argv, i);
	if (!n_option)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

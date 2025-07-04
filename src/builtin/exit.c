/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:50 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/04 18:00:13 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// exit　完成？

static int	is_numeric_argument(const char *s)
{
	int			i;
	int			len;
	const char	*llong_max = "9223372036854775807";
	const char	*llong_min = "9223372036854775808";

	i = 0;
	len = 0;
	if (!s || !*s)
		return (0);
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	len = ft_strlen(s);
	if (s[0] == '-')
	{
		if (len - 1 > 19 || (len - 1 == 19 && ft_strncmp(&s[1], llong_min, 19) > 0))
			return (0);
	}
	else
	{
		if (s[0] == '+')
		{
			if (len - 1 > 19 || (len - 1 == 19 && ft_strncmp(&s[1], llong_max, 19) > 0))
				return (0);
		}
		else
		{
			if (len > 19 || (len == 19 && ft_strncmp(s, llong_max, 19) > 0))
				return (0);
		}
	}
	return (1);
}

int	builtin_exit(char **argv)
{
	int			argc;
	long long	status;

	argc = arrlen(argv);
	ft_putendl_fd("exit", STDERR_FILENO);
	if (argc == 1)
	{
		exit(g_vars.exit_status);
	}
	if (!is_numeric_argument(argv[1]))
	{
		ft_dprintf(STDERR_FILENO, \
			"minishell: exit: %s: numeric argument required\n", argv[1]);
		exit(2);
	}
	if (argc > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		return (1);
	}
	status = ft_atoi(argv[1]);
	exit((unsigned char)status);
}

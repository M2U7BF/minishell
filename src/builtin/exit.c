/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:50 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 11:06:11 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_digit_string(const char *s)
{
	if (!s || !*s)
		return (0);
	if (*s == '+' || *s == '-')
		s++;
	if (!*s)
		return (0);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

static int	is_within_llong_range(const char *s)
{
	int			len;
	const char	*llong_max = "9223372036854775807";
	const char	*llong_min = "9223372036854775808";

	len = ft_strlen(s);
	if (*s == '-')
		return ((len - 1 < 19)
			|| (len - 1 == 19 && ft_strncmp(s + 1, llong_min, 19) <= 0));
	if (*s == '+')
		return ((len - 1 < 19)
			|| (len - 1 == 19 && ft_strncmp(s + 1, llong_max, 19) <= 0));
	return ((len < 19) || (len == 19 && ft_strncmp(s, llong_max, 19) <= 0));
}

static int	is_numeric_argument(const char *s)
{
	if (!is_valid_digit_string(s))
		return (0);
	if (!is_within_llong_range(s))
		return (0);
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

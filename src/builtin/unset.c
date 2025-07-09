/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:57 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/09 14:53:17 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(char **argv)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: unset: `%s': not a valid identifier\n", argv[i]);
			status = 1;
		}
		else
		{
			unset_env_var(&g_vars.env_list, argv[i]);
		}
		i++;
	}
	return (status);
}

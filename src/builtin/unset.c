/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:57 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/25 13:47:40 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern char	**environ;

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

static int	find_env_var_index(const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (environ[i])
	{
		if (ft_strncmp(environ[i], name, len) == 0)
		{
			if (environ[i][len] == '=' || environ[i][len] == '\0')
				return (i);
		}
		i++;
	}
	return (-1);
}

static int	remove_env_var_by_index(int index_to_remove)
{
	int		i;
	int		j;
	int		count;
	char	**new_environ;

	count = 0;
	while (environ[count])
		count++;
	new_environ = malloc(sizeof(char *) * count);
	if (!new_environ)
		return (-1);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != index_to_remove)
		{
			new_environ[j] = environ[i];
			j++;
		}
		i++;
	}
	new_environ[j] = NULL;
	free(environ[index_to_remove]);
	free(environ);
	environ = new_environ;
	return (0);
}

int	builtin_unset(char **argv)
{
	int	i;
	int	status;
	int	found_index;

	i = 1;
	status = 0;
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
			found_index = find_env_var_index(argv[i]);
			if (found_index != -1)
			{
				if (remove_env_var_by_index(found_index) != 0)
					status = 1;
			}
		}
		i++;
	}
	return (status);
}

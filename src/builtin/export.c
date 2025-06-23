/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:52 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/23 17:06:59 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_identifier(const char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	s++;
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

static void	print_sorted_env(void)
{
	extern char	**environ;
	int			count;
	char		**sorted_env;
	int			i;
	int			j;
	char		*temp;

	count = arrlen(environ);
	sorted_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
	{
		perror("minishell: export");
		return ;
	}
	i = -1;
	while (++i < count)
	{
		sorted_env[i] = ft_strdup(environ[i]);
		if (!sorted_env[i])
		{
			perror("minishell: export");
			free_str_array(sorted_env);
			return ;
		}
	}
	sorted_env[count] = NULL;
	i = -1;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - i - 1)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1], -1) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
		}
	}
	i = -1;
	while (++i < count)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		char *eq_ptr = ft_strchr(sorted_env[i], '=');
		if (eq_ptr)
		{
			write(STDOUT_FILENO, sorted_env[i], eq_ptr - sorted_env[i] + 1);
			ft_putchar_fd('"', STDOUT_FILENO);
			ft_putstr_fd(eq_ptr + 1, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		else
			ft_putendl_fd(sorted_env[i], STDOUT_FILENO);
	}
	free_str_array(sorted_env);
}

int	builtin_export(char **argv)
{
	int		i;
	int		status;
	char	*key;
	char	*eq_ptr;

	i = 1;
	status = 0;
	if (argv[1] == NULL)
	{
		print_sorted_env();
		return (0);
	}
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_dprintf(STDERR_FILENO, \
				"minishell: export: `%s': not a valid identifier\n", argv[i]);
			status = 1;
		}
		else
		{
			eq_ptr = ft_strchr(argv[i], '=');
			if (eq_ptr)
			{
				key = ft_substr(argv[i], 0, eq_ptr - argv[i]);
				if (!key)
				{
					perror("minishell: export");
					status = 1;
					i++;
					continue ;
				}
				if (setenv(key, eq_ptr + 1, 1) != 0)
				{
					perror("minishell: export");
					status = 1;
				}
				free(key);
			}
		}
		i++;
	}
	return (status);
}


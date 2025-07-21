/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:52 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 16:56:49 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_env_entry(const char *entry)
{
	char	*eq_ptr;
	char	*key;
	char	*value;

	eq_ptr = ft_strchr(entry, '=');
	if (eq_ptr)
	{
		key = ft_substr(entry, 0, eq_ptr - entry);
		value = eq_ptr + 1;
		ft_dprintf(STDOUT_FILENO, "declare -x %s=\"%s\"\n", key, value);
		free(key);
	}
	else
	{
		ft_dprintf(STDOUT_FILENO, "declare -x %s\n", entry);
	}
}

static void	print_sorted_env(t_list *env_list)
{
	char	**env_array;
	int		i;

	env_array = convert_env_list_to_array(env_list);
	if (!env_array)
		return ;
	sort_env_array(env_array);
	i = 0;
	while (env_array[i])
	{
		print_env_entry(env_array[i]);
		i++;
	}
	free_str_array(&env_array);
}

static int	handle_invalid_identifier(const char *arg)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: export: `%s': not a valid identifier\n", arg);
	return (1);
}

static void	handle_valid_export(const char *arg)
{
	char	*eq_ptr;
	char	*key;
	char	*value;
	t_list	*env_list;

	env_list = access_env_list(false, NULL);
	eq_ptr = ft_strchr(arg, '=');
	if (eq_ptr)
	{
		key = ft_substr(arg, 0, eq_ptr - arg);
		value = eq_ptr + 1;
		set_env_var(&env_list, key, value);
		free(key);
	}
	else
	{
		set_env_var(&env_list, arg, "");
	}
}

int	builtin_export(char **argv)
{
	int	i;
	int	status;

	if (argv[1] == NULL)
	{
		print_sorted_env(access_env_list(false, NULL));
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!is_valid_export(argv[i]))
			status |= handle_invalid_identifier(argv[i]);
		else
			handle_valid_export(argv[i]);
		i++;
	}
	return (status);
}

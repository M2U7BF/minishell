/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:07:31 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 16:56:16 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_valid_assignment_key(const char *s, size_t len)
{
	size_t	i;

	if (len == 0)
		return (false);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (false);
	i = 1;
	while (i < len)
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

bool	is_variable_assignment(const char *arg)
{
	char	*eq_ptr;
	char	*plus_eq_ptr;

	eq_ptr = ft_strchr(arg, '=');
	if (eq_ptr == NULL || eq_ptr == arg)
		return (false);
	plus_eq_ptr = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus_eq_ptr != NULL && plus_eq_ptr + 1 == eq_ptr)
	{
		if (plus_eq_ptr == arg)
			return (false);
		return (is_valid_assignment_key(arg, plus_eq_ptr - arg));
	}
	return (is_valid_assignment_key(arg, eq_ptr - arg));
}

static void	handle_plus_assignment(const char *key, const char *value)
{
	char	*old_value;
	char	*new_value;
	t_list	*env_list;

	env_list = access_env_list(false, NULL);
	old_value = get_env_value(env_list, key);
	if (old_value)
	{
		new_value = ft_strjoin(old_value, value);
		set_env_var(&env_list, key, new_value);
		free(new_value);
	}
	else
		set_env_var(&env_list, key, value);
}

void	handle_variable_assignment(const char *arg)
{
	char		*eq_ptr;
	char		*plus_eq_ptr;
	char		*key;
	const char	*value;
	t_list		*env_list;

	env_list = access_env_list(false, NULL);
	eq_ptr = ft_strchr(arg, '=');
	plus_eq_ptr = ft_strnstr(arg, "+=", ft_strlen(arg));
	value = eq_ptr + 1;
	if (plus_eq_ptr != NULL && plus_eq_ptr + 1 == eq_ptr)
		key = ft_substr(arg, 0, plus_eq_ptr - arg);
	else
		key = ft_substr(arg, 0, eq_ptr - arg);
	if (!key)
		return ;
	if (plus_eq_ptr != NULL && plus_eq_ptr + 1 == eq_ptr)
		handle_plus_assignment(key, value);
	else
		set_env_var(&env_list, key, value);
	free(key);
	access_exit_status(true, EXIT_SUCCESS);
}

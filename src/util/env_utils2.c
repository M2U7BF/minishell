/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:39:29 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 17:58:44 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*create_env_var(const char *env_str)
{
	t_env	*env_var;
	char	*eq_ptr;

	env_var = ft_calloc(1, sizeof(t_env));
	if (!env_var)
		libc_error();
	eq_ptr = ft_strchr(env_str, '=');
	if (eq_ptr)
	{
		env_var->key = ft_substr(env_str, 0, eq_ptr - env_str);
		env_var->value = ft_strdup(eq_ptr + 1);
	}
	else
	{
		env_var->key = ft_strdup(env_str);
		env_var->value = NULL;
	}
	if (!env_var->key || (eq_ptr && !env_var->value))
		libc_error();
	return (env_var);
}

void	del_env_var(void *content)
{
	t_env	*env_var;

	env_var = (t_env *)content;
	ft_free((void **)&env_var->key);
	ft_free((void **)&env_var->value);
	ft_free((void **)&env_var);
}

void	free_env_list(t_list **env_list)
{
	ft_lstclear(env_list, del_env_var);
	*env_list = NULL;
}

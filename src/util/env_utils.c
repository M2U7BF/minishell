/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:42:04 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 17:58:59 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*env_to_string(t_env *env_var)
{
	char	*tmp;
	char	*result;

	if (env_var->value)
	{
		tmp = ft_strjoin(env_var->key, "=");
		if (!tmp)
			libc_error();
		result = ft_strjoin(tmp, env_var->value);
		free(tmp);
	}
	else
	{
		result = ft_strdup(env_var->key);
	}
	if (!result)
		libc_error();
	return (result);
}

char	**convert_env_list_to_array(t_list *env_list)
{
	char	**array;
	t_list	*current;
	t_env	*env_var;
	int		i;

	array = ft_calloc(ft_lstsize(env_list) + 1, sizeof(char *));
	if (!array)
		libc_error();
	current = env_list;
	i = 0;
	while (current)
	{
		env_var = (t_env *)current->content;
		array[i++] = env_to_string(env_var);
		current = current->next;
	}
	return (array);
}

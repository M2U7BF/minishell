/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:32 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/15 17:57:51 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_env_list(t_list **env_list, char **environ)
{
	int		i;
	t_env	*env_var;

	*env_list = NULL;
	i = 0;
	while (environ[i])
	{
		env_var = create_env_var(environ[i]);
		ft_lstadd_back(env_list, ft_lstnew(env_var));
		i++;
	}
}

char	*get_env_value(t_list *env_list, const char *key)
{
	t_list	*current;
	t_env	*env_var;

	current = env_list;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (ft_strncmp(env_var->key, key, ft_strlen(key) + 1) == 0)
			return (env_var->value);
		current = current->next;
	}
	return (NULL);
}

static int	update_env_var(t_list *env_list, const char *key, const char *value)
{
	t_env	*env_var;

	while (env_list)
	{
		env_var = (t_env *)env_list->content;
		if (ft_strncmp(env_var->key, key, ft_strlen(key) + 1) == 0)
		{
			free(env_var->value);
			env_var->value = ft_strdup(value);
			if (!env_var->value)
				libc_error("1");
			return (1);
		}
		env_list = env_list->next;
	}
	return (0);
}

void	set_env_var(t_list **env_list, const char *key, const char *value)
{
	t_env	*new_var;

	if (update_env_var(*env_list, key, value))
		return ;
	new_var = ft_calloc(1, sizeof(t_env));
	if (!new_var)
		libc_error("1");
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	if (!new_var->key || !new_var->value)
		libc_error("1");
	ft_lstadd_back(env_list, ft_lstnew(new_var));
}

void	unset_env_var(t_list **env_list, const char *key)
{
	t_list	*current;
	t_list	*prev;
	t_env	*env_var;

	prev = NULL;
	current = *env_list;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (ft_strncmp(env_var->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev == NULL)
				*env_list = current->next;
			else
				prev->next = current->next;
			ft_lstdelone(current, del_env_var);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

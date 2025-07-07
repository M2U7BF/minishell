/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:42:04 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 13:18:10 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// env
static t_env	*create_env_var(const char *env_str)
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
		env_var->value = NULL; // export a のような場合
	}
	if (!env_var->key || (eq_ptr && !env_var->value))
		libc_error();
	return (env_var);
}

static void	del_env_var(void *content)
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

void	set_env_var(t_list **env_list, const char *key, const char *value)
{
	t_list	*current;
	t_env	*env_var;
	t_env	*new_var;

	current = *env_list;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (ft_strncmp(env_var->key, key, ft_strlen(key) + 1) == 0)
		{
			free(env_var->value);
			env_var->value = ft_strdup(value);
			if (!env_var->value)
				libc_error();
			return ;
		}
		current = current->next;
	}
	// 見つからなかった場合は新規追加
	new_var = ft_calloc(1, sizeof(t_env));
	if (!new_var)
		libc_error();
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	if (!new_var->key || !new_var->value)
		libc_error();
	ft_lstadd_back(env_list, ft_lstnew(new_var));
}

void	unset_env_var(t_list **env_list, const char *key)
{
	t_list	*current;
	t_list	*prev;
	t_env	*env_var;

	current = *env_list;
	prev = NULL;
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

char	**convert_env_list_to_array(t_list *env_list)
{
	char	**array;
	t_list	*current;
	t_env	*env_var;
	int		i;
	char	*tmp;

	array = ft_calloc(ft_lstsize(env_list) + 1, sizeof(char *));
	if (!array)
		libc_error();
	current = env_list;
	i = 0;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (env_var->value)
		{
			tmp = ft_strjoin(env_var->key, "=");
			array[i] = ft_strjoin(tmp, env_var->value);
			free(tmp);
		}
		else // valueがない場合 (e.g. export a)
		{
			array[i] = ft_strdup(env_var->key);
		}
		if (!array[i])
			libc_error();
		i++;
		current = current->next;
	}
	return (array);
}

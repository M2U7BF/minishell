/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:57:47 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/19 16:13:44 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_dotdot(char *resolved)
{
	char	*last_slash;

	last_slash = ft_strrchr(resolved, '/');
	if (last_slash > resolved)
		*last_slash = '\0';
	else if (last_slash == resolved && *(resolved + 1))
		*(resolved + 1) = '\0';
}

static void	append_path(char **resolved, const char *component)
{
	char	*temp;

	temp = *resolved;
	if (temp[ft_strlen(temp) - 1] != '/')
	{
		*resolved = ft_strjoin(temp, "/");
		free(temp);
		temp = *resolved;
	}
	*resolved = ft_strjoin(temp, component);
	free(temp);
}

char	*resolve_path(const char *rel_path, t_list *env_list)
{
	char	*pwd;
	char	*resolved;
	char	**components;
	int		i;

	pwd = get_env_value(env_list, "PWD");
	resolved = NULL;
	if (pwd)
		resolved = ft_strdup(pwd);
	if (!resolved)
		return (NULL);
	components = ft_split(rel_path, '/');
	if (!components)
	{
		free(resolved);
		return (NULL);
	}
	i = -1;
	while (components[++i])
	{
		if (!ft_strncmp(components[i], "..", 3))
			handle_dotdot(resolved);
		else if (ft_strncmp(components[i], ".", 2) && *components[i])
			append_path(&resolved, components[i]);
	}
	free_str_array(&components);
	return (resolved);
}

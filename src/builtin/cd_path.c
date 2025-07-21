/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:57:47 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 12:02:06 by atashiro         ###   ########.fr       */
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

static char	*get_resolved_base_path(t_list *env_list)
{
	char	*pwd;

	pwd = get_env_value(env_list, "PWD");
	if (!pwd)
		return (NULL);
	return (ft_strdup(pwd));
}

static int	process_components(char **components, char **resolved)
{
	int	i;

	i = 0;
	while (components[i])
	{
		if (!ft_strncmp(components[i], "..", 3))
			handle_dotdot(*resolved);
		else if (ft_strncmp(components[i], ".", 2) && *components[i])
			append_path(resolved, components[i]);
		i++;
	}
	return (0);
}

char	*resolve_path(const char *rel_path, t_list *env_list)
{
	char	*resolved;
	char	**components;

	resolved = get_resolved_base_path(env_list);
	if (!resolved)
		return (NULL);
	components = ft_split(rel_path, '/');
	if (!components)
	{
		free(resolved);
		return (NULL);
	}
	process_components(components, &resolved);
	free_str_array(&components);
	return (resolved);
}

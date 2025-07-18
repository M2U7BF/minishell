/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:59:14 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/18 13:39:50 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free(void **p)
{
	free(*p);
	*p = NULL;
}

bool	is_symlink(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
		libc_error();
	return (S_ISLNK(sb.st_mode));
}

void	get_abs_path(const char *path, char *buf)
{
	char				*current;

	if (!path)
		return ;
	if (path[0] == '/')
		ft_strlcpy(buf, path, ft_strlen(path) + 1);
	else
	{
		current = get_env_value(g_vars.env_list, "PWD");
		if (path[0] == '.' && path[1] == '/')
			path = ft_substr(path, 2, ft_strlen(path) - 2);
		ft_strlcpy(buf, current, ft_strlen(current) + 1);
		ft_strlcpy(buf + ft_strlen(current), "/", 2);
		if (path[ft_strlen(path) - 1] == '/')
			ft_strlcpy(buf + ft_strlen(current) + 1, path, ft_strlen(path));
		else
			ft_strlcpy(buf + ft_strlen(current) + 1, path, ft_strlen(path) + 1);
	}
}

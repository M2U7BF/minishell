/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:59:14 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/18 12:54:17 by kkamei           ###   ########.fr       */
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

const char	*get_abs_path(const char *path, char *buf)
{
	char				*current;
	static const char	*abs_path[] = {"", "/", "", NULL};

	if (!path)
		return (NULL);
	if (path[0] == '/')
		return (path);
	current = get_env_value(g_vars.env_list, "PWD");
	if (path[0] == '.' && path[1] == '/')
		path = ft_substr(path, 2, ft_strlen(path) - 2);
	abs_path[0] = current;
	abs_path[2] = path;
	ft_strlcpy(buf, current, ft_strlen(current) + 1);
	ft_strlcpy(buf + ft_strlen(current), "/", 2);
  if (path[ft_strlen(path) - 1] == '/')
    ft_strlcpy(buf + ft_strlen(current) + 1, path, ft_strlen(path));
  else
	  ft_strlcpy(buf + ft_strlen(current) + 1, path, ft_strlen(path) + 1);
  ft_dprintf(STDERR_FILENO, "path:[%s]\n", buf);
	return (buf);
}

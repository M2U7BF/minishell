/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:50:35 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/15 17:57:51 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_hostname(t_list *env_list)
{
	char	*session_manager;
	char	*hostname_start;
	char	*hostname_end;
	char	*hostname;

	session_manager = get_env_value(env_list, "SESSION_MANAGER");
	if (!session_manager)
		return (NULL);
	hostname_start = ft_strnstr(session_manager,
			"local/", ft_strlen(session_manager));
	if (!hostname_start)
		return (NULL);
	hostname_start += ft_strlen("local/");
	hostname_end = ft_strchr(hostname_start, '.');
	if (!hostname_end)
		return (NULL);
	hostname = ft_substr(hostname_start, 0, hostname_end - hostname_start);
	if (!hostname)
		libc_error("1");
	return (hostname);
}

char	*get_display_path(char *cwd, char *home)
{
	char	*display_path;

	if (home && ft_strncmp(cwd, home, ft_strlen(home)) == 0)
	{
		display_path = ft_strjoin("~", cwd + ft_strlen(home));
		if (!display_path)
			libc_error("1");
		return (display_path);
	}
	return (ft_strdup(cwd));
}

char	*build_user_host(char *user, char *hostname)
{
	char	*user_host;
	char	*tmp;

	if (hostname)
	{
		tmp = ft_strjoin(user, "@");
		if (!tmp)
			libc_error("1");
		user_host = ft_strjoin(tmp, hostname);
		ft_free((void **)&tmp);
		ft_free((void **)&hostname);
		if (!user_host)
			libc_error("1");
	}
	else
	{
		user_host = ft_strdup(user);
		if (!user_host)
			libc_error("1");
	}
	return (user_host);
}

char	*get_display_path_str(void)
{
	char	cwd_buf[1024];
	char	*cwd;
	char	*home;
	char	*display_path;

	cwd = getcwd(cwd_buf, sizeof(cwd_buf));
	if (!cwd)
		return (NULL);
	home = get_env_value(g_vars.env_list, "HOME");
	display_path = get_display_path(cwd, home);
	return (display_path);
}

char	*assemble_prompt(char *user, char *path)
{
	char	*tmp;
	char	*prompt;

	tmp = ft_strjoin(user, ":");
	if (!tmp)
		libc_error("1");
	prompt = ft_strjoin(tmp, path);
	ft_free((void **)&tmp);
	if (!prompt)
		libc_error("1");
	tmp = prompt;
	prompt = ft_strjoin(tmp, "$> ");
	ft_free((void **)&tmp);
	if (!prompt)
		libc_error("1");
	return (prompt);
}

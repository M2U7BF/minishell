/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:21:03 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 16:53:22 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_prompt(void)
{
	char	*user;
	char	*hostname;
	char	*display_path;
	char	*user_host;
	char	*prompt;

	user = get_env_value(access_env_list(false, NULL), "USER");
	if (!user)
		user = "user";
	hostname = get_hostname(access_env_list(false, NULL));
	display_path = get_display_path_str();
	if (!display_path)
		return (ft_strdup("minishell$ "));
	user_host = build_user_host(user, hostname);
	if (!user_host)
		libc_error();
	prompt = assemble_prompt(user_host, display_path);
	ft_free((void **)&user_host);
	ft_free((void **)&display_path);
	return (prompt);
}

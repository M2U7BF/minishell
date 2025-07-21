/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:59:14 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 17:09:12 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free(void **p)
{
	free(*p);
	*p = NULL;
}

int	access_exit_status(bool is_update, int value)
{
	static int	exit_status = 0;

	if (is_update)
		exit_status = value;
	return (exit_status);
}

t_list	*access_env_list(bool is_update, t_list *value)
{
	static t_list	*env_list = NULL;

	if (is_update)
		env_list = value;
	return (env_list);
}

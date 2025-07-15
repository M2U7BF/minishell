/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:47 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/15 15:45:36 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(void)
{
	t_list	*current;
	t_env	*env_var;

	current = g_vars.env_list;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (env_var->value != NULL)
		{
			ft_putstr_fd(env_var->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(env_var->value, STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:47 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/21 16:52:26 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(void)
{
	t_list	*current;
	t_env	*env_var;

	current = access_env_list(false, NULL);
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

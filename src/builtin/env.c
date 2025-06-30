/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:47 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/30 12:26:01 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(t_list *env_list)
{
	t_list	*current;
	t_env	*env_var;

	current = env_list;
	while (current)
	{
		env_var = (t_env *)current->content;
		// value がある環境変数のみ表示
		if (env_var->value != NULL && *(env_var->value) != '\0')
		{
			ft_putstr_fd(env_var->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(env_var->value, STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}

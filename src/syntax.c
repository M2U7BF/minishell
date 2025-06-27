/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:24:48 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 12:04:52 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_redirection_syntax_error(t_token *token_list)
{
	t_token	*current;
	t_token	*prev;

	prev = NULL;
	current = token_list;
	while (current)
	{
		if (current->type == REDIRECTION)
		{
			if (ft_strncmp(current->str, "<", 2) == 0
				|| ft_strncmp(current->str, "<<", 3) == 0)
			{
				if (!current->next)
					return (true);
				if (current->next && current->next->type != WORD
					&& current->next->type != DELIMITER)
					return (true);
			}
			else if (ft_strncmp(current->str, ">", 2) == 0
				|| ft_strncmp(current->str, ">>", 3) == 0)
			{
				if (!current->next || current->next->type != WORD)
					return (true);
			}
		}
		prev = current;
		current = current->next;
	}
	return (false);
}

bool	is_syntax_error(t_token *token_list)
{
	if (!token_list)
		return (true);
	if (is_redirection_syntax_error(token_list))
		return (true);
	return (false);
}

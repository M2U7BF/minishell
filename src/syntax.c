/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:24:48 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 10:38:48 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_redirection_syntax_error(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	while (current)
	{
		if (current->type == REDIR)
		{
			if (ft_strncmp(current->str, "<", 1) == 0)
			{
				if (!current->next)
					return (true);
				else if (current->next->type != WORD
					&& current->next->type != DELIM)
					return (true);
			}
			else
			{
				if (!current->next || current->next->type != WORD)
					return (true);
			}
		}
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

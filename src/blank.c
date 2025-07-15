/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blank.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:36:48 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 12:09:52 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_blank(t_token *token_list)
{
	t_token	*current_token;
	t_token	*old;

	current_token = token_list;
	while (current_token)
	{
		if (current_token->type == BLANK)
		{
			old = current_token;
			current_token = current_token->next;
			del_token(&token_list, old);
		}
		else
			current_token = current_token->next;
	}
}

// Convert strings enclosed in quotes to WORD type
void	process_str_quoted(t_token *token_list)
{
	t_token	*current_token;
	char	current_quote;
	int		i;

	current_quote = '\0';
	current_token = token_list;
	while (current_token)
	{
		if (current_token->type != WORD && current_quote)
			current_token->type = WORD;
		else if (current_token->type == WORD)
		{
			i = -1;
			while (current_token->str[++i])
			{
				if (!current_quote && is_quote(current_token->str[i]))
					current_quote = current_token->str[i];
				else if (current_quote
					&& current_quote == current_token->str[i])
					current_quote = '\0';
			}
		}
		current_token = current_token->next;
	}
}

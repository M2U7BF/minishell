/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:23:36 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 12:12:09 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Insert 'add' after the current token
// If token is NULL, 'add' becomes the first element
void	insert_token(t_token **first_token, t_token *token, t_token *add)
{
	t_token	*next_token;

	if (!add)
		return ;
	if (!token)
	{
		add->next = *first_token;
		*first_token = add;
	}
	else
	{
		next_token = token->next;
		token->next = add;
		add->next = next_token;
	}
}

t_token	*get_prev_token(t_token **token_list, t_token *token)
{
	t_token	*current_token;
	t_token	*prev;

	if (*token_list == token)
		return (NULL);
	current_token = *token_list;
	prev = NULL;
	while (current_token)
	{
		if (current_token == token)
			return (prev);
		prev = current_token;
		current_token = current_token->next;
	}
	return (NULL);
}

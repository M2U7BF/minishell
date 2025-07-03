/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:23:36 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 13:24:40 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// tokenの次の要素にaddを挿入する。
// tokenがNULLなら、addが先頭になる。
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

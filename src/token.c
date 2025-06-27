/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:31:55 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 16:05:32 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(char *str, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->next = NULL;
	token->str = str;
	token->type = type;
	return (token);
}

// トークンのリストにトークンを追加する。
// リストがNULLの場合は、与えられたトークンが先頭になる。
void	append_token(t_token **token_list, t_token *token)
{
	t_token	*current_token;

	if (!token)
		return ;
	if (!token_list || !(*token_list))
	{
		(*token_list) = token;
		return ;
	}
	current_token = (*token_list);
	while (current_token)
	{
		if (!current_token->next)
		{
			current_token->next = token;
			break ;
		}
		current_token = current_token->next;
	}
}

t_token	*token_dup(t_token *token)
{
	t_token	*result;

	if (!token)
		return (NULL);
	if (token->str)
		result = create_token(ft_strdup(token->str), token->type);
	else
		result = create_token(NULL, token->type);
	return (result);
}

void	del_token(t_token **token_list, t_token *token)
{
	t_token	*current;
	t_token	*prev;

	current = *token_list;
	prev = NULL;
	if (!token_list || !*token_list || !token)
		return ;
	if (*token_list == token)
		*token_list = token->next;
	else
	{
		while (current && current != token)
		{
			prev = current;
			current = current->next;
		}
		if (current == token)
			prev->next = token->next;
	}
	ft_free(token->str);
	ft_free(token);
}

void	free_token_list(t_token *token_list)
{
	t_token	*current_token;
	t_token	*tmp;

	if (!token_list)
		return ;
	current_token = token_list;
	while (current_token)
	{
		tmp = current_token;
		current_token = current_token->next;
		ft_free(tmp->str);
		ft_free(tmp);
	}
}

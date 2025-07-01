/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:48:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 12:35:32 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	token_list_len(t_token *token_list)
{
	t_token	*current_token;
	int		i;

	if (!token_list)
		return (0);
	current_token = token_list;
	i = 0;
	while (current_token)
	{
		i++;
		current_token = current_token->next;
	}
	return (i);
}

// 隣接するWORDタイプのトークンを結合する
t_token	*join_tokens(t_token *token_list)
{
	t_token	*current;
	t_token	*old;
	char	*s;

	current = token_list;
	while (current && current->next)
	{
		if (current->type == WORD && current->next->type == WORD)
		{
			s = malloc(sizeof(char) * (ft_strlen(current->str)
						+ ft_strlen(current->next->str) + 1));
			ft_memcpy(s, current->str, ft_strlen(current->str));
			ft_memcpy(s + ft_strlen(current->str), current->next->str,
				ft_strlen(current->next->str));
			s[ft_strlen(current->str) + ft_strlen(current->next->str)] = '\0';
			old = current;
			current = create_token(s, WORD);
			insert_token(&token_list, old->next, current);
			del_token(&token_list, old->next);
			del_token(&token_list, old);
		}
		else
			current = current->next;
	}
	return (token_list);
}

char	**tokens_to_arr(t_token *token_list)
{
	int		i;
	char	**arr;
	t_token	*current_token;

	i = 0;
	if (!token_list)
		return (NULL);
	arr = malloc(sizeof(char *) * (token_list_len(token_list) + 1));
	if (!arr)
		return (NULL);
	current_token = token_list;
	while (current_token)
	{
		if (current_token->str)
			arr[i++] = ft_strdup(current_token->str);
		current_token = current_token->next;
	}
	arr[i] = NULL;
	return (arr);
}

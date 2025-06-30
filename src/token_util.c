/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:48:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/30 15:00:22 by kkamei           ###   ########.fr       */
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
	t_token	*prev;
	t_token	*old;
	char	*s1;

	current = token_list;
	prev = NULL;
	while (current)
	{
		if (current && prev && prev->type == WORD && current->type == WORD)
		{
			s1 = NULL;
			old = current;
			current = get_prev_token(&token_list, prev);
			s1 = malloc(sizeof(char) * (ft_strlen(prev->str)
						+ ft_strlen(old->str) + 1));
			ft_memcpy(s1, prev->str, ft_strlen(prev->str));
			ft_memcpy(s1 + ft_strlen(prev->str), old->str, ft_strlen(old->str));
			s1[ft_strlen(prev->str) + ft_strlen(old->str)] = '\0';
			del_token(&token_list, old);
			del_token(&token_list, prev);
			insert_token(&token_list, current, create_token(s1, WORD));
		}
		prev = current;
		if (!current)
			current = token_list;
		else
			current = current->next;
	}
	return (token_list);
}

char	**tokens_to_arr(t_token *token_list)
{
	int		i;
	int		null_count;
	int		len;
	char	**arr;
	t_token	*current_token;

	null_count = 0;
	if (!token_list)
		return (0);
	len = token_list_len(token_list);
	current_token = token_list;
	while (current_token)
	{
		if (!current_token->str)
			null_count++;
		current_token = current_token->next;
	}
	arr = malloc(sizeof(char *) * (len + 1 - null_count));
	if (!arr)
		return (NULL);
	i = 0;
	current_token = token_list;
	while (current_token)
	{
		if (!current_token->str)
		{
			current_token = current_token->next;
			continue ;
		}
		else
			arr[i] = ft_strdup(current_token->str);
		current_token = current_token->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

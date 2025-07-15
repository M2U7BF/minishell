/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:48:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/14 11:39:46 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	token_list_len(t_token *token_list)
{
	t_token	*cur_token;
	int		i;

	if (!token_list)
		return (0);
	cur_token = token_list;
	i = 0;
	while (cur_token)
	{
		i++;
		cur_token = cur_token->next;
	}
	return (i);
}

// 隣接するWORDタイプのトークンを結合する
t_token	*join_tokens(t_token *token_list)
{
	t_token	*cur;
	t_token	*old;
	char	*s;

	cur = token_list;
	while (cur && cur->next)
	{
		if (cur->type == WORD && cur->next->type == WORD)
		{
			s = malloc(sizeof(char) * (ft_strlen(cur->str)
						+ ft_strlen(cur->next->str) + 1));
			ft_memcpy(s, cur->str, ft_strlen(cur->str));
			ft_memcpy(s + ft_strlen(cur->str), cur->next->str,
				ft_strlen(cur->next->str));
			s[ft_strlen(cur->str) + ft_strlen(cur->next->str)] = '\0';
			old = cur;
			cur = create_token(s, WORD);
			insert_token(&token_list, old->next, cur);
			del_token(&token_list, old->next);
			del_token(&token_list, old);
		}
		else
			cur = cur->next;
	}
	return (token_list);
}

char	**tokens_to_arr(t_token *token_list)
{
	int		i;
	char	**arr;
	t_token	*cur_token;

	i = 0;
	if (!token_list)
		return (NULL);
	arr = malloc(sizeof(char *) * (token_list_len(token_list) + 1));
	if (!arr)
		return (NULL);
	cur_token = token_list;
	while (cur_token)
	{
		if (cur_token->str)
			arr[i++] = ft_strdup(cur_token->str);
		cur_token = cur_token->next;
	}
	arr[i] = NULL;
	return (arr);
}

bool	is_redir_pair(t_token *token)
{
	return (token->type == REDIR && (token->next->type == WORD
			|| token->next->type == DELIM));
}

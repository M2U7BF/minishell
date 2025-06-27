/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:48:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 13:20:11 by kkamei           ###   ########.fr       */
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

// BLANK, REDIRECTION, を挟まずに隣り合う文字列を結合する。
t_token	*join_tokens(t_token *token_list)
{
	t_list	*tmp;
	t_token	*current;
	t_token	*prev;
	t_token	*old;
	t_token	*old_prev;
	char	**tmp_arr;
	char	*tmp_str;

	current = token_list;
	prev = NULL;
	tmp = NULL;
	while (current)
	{
		if (current != NULL && prev != NULL && prev->type == WORD
			&& current->type == WORD)
		{
			old_prev = prev;
			old = current;
			current = get_prev_token(&token_list, old_prev);
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old_prev->str)));
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
			del_token(&token_list, old);
			del_token(&token_list, old_prev);
			tmp_arr = lst_to_str_arr(tmp);
			tmp_str = ft_strjoin_all(tmp_arr);
			free_str_array(tmp_arr);
			ft_lstclear(&tmp, del_content);
			insert_token(&token_list, current, create_token(tmp_str, WORD));
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
	int i;
	int null_count;
	int len;
	char **arr;
	t_token *current_token;

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
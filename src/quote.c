/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:40:49 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 12:42:45 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// シングルクォートが隣接する文字列を探し、隣接するシングルクォートとその文字列を1つの要素にまとめる
t_token	*process_single_quote(t_token *token_list)
{
	t_list	*tmp;
	t_token	*current_token;
	t_token	*old;
	t_token	*old_next;
	t_token	*old_prev;
	int		single_quote_count;
	char	**tmp_arr;
	char	*tmp_str;

	current_token = token_list;
	single_quote_count = 0;
	tmp = NULL;
	while (current_token)
	{
		if (ft_strncmp(current_token->str, "\'", 2) == 0)
		{
			if (single_quote_count % 2 == 0 && current_token->next)
			{
				if (ft_strncmp(current_token->next->str, "\'", 2) == 0)
					single_quote_count++;
				old = current_token;
				old_next = current_token->next;
				old_prev = get_prev_token(&token_list, old);
				ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
				ft_lstadd_back(&tmp,
					ft_lstnew((void *)ft_strdup(old_next->str)));
				del_token(&token_list, old);
				del_token(&token_list, old_next);
				tmp_arr = lst_to_str_arr(tmp);
				tmp_str = ft_strjoin_all(tmp_arr);
				free_str_array(tmp_arr);
				ft_lstclear(&tmp, del_content);
				current_token = create_token(tmp_str, WORD);
				insert_token(&token_list, old_prev, current_token);
			}
			single_quote_count++;
		}
		else if (single_quote_count % 2 == 1 && current_token->next
			&& ft_strncmp(current_token->next->str, "\'", 2) == 0)
		{
			old = current_token;
			old_next = current_token->next;
			old_prev = get_prev_token(&token_list, old);
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old_next->str)));
			del_token(&token_list, old);
			del_token(&token_list, old_next);
			tmp_arr = lst_to_str_arr(tmp);
			tmp_str = ft_strjoin_all(tmp_arr);
			free_str_array(tmp_arr);
			ft_lstclear(&tmp, del_content);
			current_token = create_token(tmp_str, WORD);
			insert_token(&token_list, old_prev, current_token);
			single_quote_count++;
		}
		else
			current_token = current_token->next;
	}
	return (token_list);
}

// ダブルクォートが隣接する文字列を探し、隣接するダブルクォートとその文字列を1つの要素にまとめる
t_token	*process_double_quote(t_token *token_list)
{
	t_list	*tmp;
	t_token	*current_token;
	t_token	*old;
	t_token	*old_next;
	t_token	*old_prev;
	int		double_quote_count;
	char	**tmp_arr;
	char	*tmp_str;
	int		i;

	current_token = token_list;
	double_quote_count = 0;
	tmp = NULL;
	i = 0;
	while (current_token)
	{
		if (ft_strncmp(current_token->str, "\"", 2) == 0)
		{
			if (double_quote_count % 2 == 0 && current_token->next)
			{
				if (ft_strncmp(current_token->next->str, "\"", 2) == 0)
					double_quote_count++;
				old = current_token;
				old_next = current_token->next;
				old_prev = get_prev_token(&token_list, old);
				ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
				ft_lstadd_back(&tmp,
					ft_lstnew((void *)ft_strdup(old_next->str)));
				del_token(&token_list, old);
				del_token(&token_list, old_next);
				tmp_arr = lst_to_str_arr(tmp);
				tmp_str = ft_strjoin_all(tmp_arr);
				free_str_array(tmp_arr);
				ft_lstclear(&tmp, del_content);
				current_token = create_token(tmp_str, WORD);
				insert_token(&token_list, old_prev, current_token);
			}
			double_quote_count++;
		}
		else if (double_quote_count % 2 == 1 && current_token->next
			&& ft_strncmp(current_token->next->str, "\"", 2) == 0)
		{
			old = current_token;
			old_next = current_token->next;
			old_prev = get_prev_token(&token_list, old);
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old_next->str)));
			del_token(&token_list, old);
			del_token(&token_list, old_next);
			tmp_arr = lst_to_str_arr(tmp);
			tmp_str = ft_strjoin_all(tmp_arr);
			free_str_array(tmp_arr);
			ft_lstclear(&tmp, del_content);
			current_token = create_token(tmp_str, WORD);
			insert_token(&token_list, old_prev, current_token);
			double_quote_count++;
		}
		else
			current_token = current_token->next;
		i++;
	}
	return (token_list);
}

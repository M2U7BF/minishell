/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:40:49 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 16:43:21 by kkamei           ###   ########.fr       */
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

bool	is_quotation_error(char *input_line)
{
	if (count_chr(input_line, '\'') % 2 == 1)
	{
		ft_dprintf(STDERR_FILENO, "Unclosed quote\n");
		return (true);
	}
	else if (count_chr(input_line, '\"') % 2 == 1)
	{
		ft_dprintf(STDERR_FILENO, "Unclosed quote\n");
		return (true);
	}
	return (false);
}

// （bashの定義と同様に）クォーテーションを削除する。
// 先頭から見て、ダブルクォートの数が偶数の場合、”ダブルクォートに囲まれていない”と判断する。
void	quote_removal(t_token *token)
{
	t_token		*current_token;
	char		**tmp;
	char		*old;
	int			double_quote_count;
	int			i;
	static char	*quote[] = {"\"", "\'", NULL};
	char		*current_quote;
	t_list		*tmp_list;
	t_list		*tmp2;
	int			str_len;
	char		**tmp_strarr;
	char		*tmp_str;

	tmp2 = NULL;
	current_token = token;
	double_quote_count = 0;
	while (current_token && current_token->str)
	{
		str_len = ft_strlen(current_token->str);
		if (current_token->type == DELIMITER)
		{
			current_token = current_token->next;
			continue ;
		}
		if (ft_strchr(current_token->str, '\'') != NULL
				|| ft_strchr(current_token->str, '\"') != NULL)
		{
			tmp = ft_multi_split_by_word_leave_separator(current_token->str,
					quote);
			i = -1;
			current_quote = NULL;
			tmp_list = NULL;
			while (tmp[++i])
			{
				if (!current_quote && (ft_strncmp("\"", tmp[i], 2) == 0
							|| ft_strncmp("\'", tmp[i], 2) == 0))
				{
					current_quote = ft_strdup(tmp[i]);
					continue ;
				}
				if (current_quote && ft_strncmp(current_quote, tmp[i], 2) == 0)
				{
					ft_free(current_quote);
					current_quote = NULL;
					tmp_strarr = lst_to_str_arr(tmp_list);
					tmp_str = ft_strjoin_all(tmp_strarr);
					free_str_array(tmp_strarr);
					if (tmp_str != NULL)
					{
						ft_lstadd_back(&tmp2, ft_lstnew((void *)tmp_str));
					}
					ft_lstclear(&tmp_list, del_content);
					continue ;
				}
				ft_lstadd_back(&tmp_list, ft_lstnew((void *)ft_strdup(tmp[i])));
			}
			if (tmp_list != NULL)
			{
				tmp_strarr = lst_to_str_arr(tmp_list);
				tmp_str = ft_strjoin_all(tmp_strarr);
				free_str_array(tmp_strarr);
				if (tmp_str != NULL)
					ft_lstadd_back(&tmp2, ft_lstnew((void *)tmp_str));
				ft_lstclear(&tmp_list, del_content);
			}
			old = current_token->str;
			tmp_strarr = lst_to_str_arr(tmp2);
			tmp_str = ft_strjoin_all(tmp_strarr);
			if (tmp_str)
				current_token->str = tmp_str;
			else
				current_token->str = ft_strdup("");
			free_str_array(tmp);
			ft_lstclear(&tmp2, del_content);
			free_str_array(tmp_strarr);
			ft_free(old);
		}
		current_token = current_token->next;
	}
}

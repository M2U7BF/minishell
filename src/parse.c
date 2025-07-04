/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 20:24:50 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_quotation_error(char *input_line)
{
	// TODO エラー文を修正。strrchrなどでエラー位置を特定する？
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
				// xクォートと一致したら、そこまでの要素を結合する
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

void	null_to_empty(t_token *token)
{
	t_token	*current_token;

	current_token = token;
	while (current_token)
	{
		if (!current_token->str)
			current_token->str = ft_strdup("");
		current_token = current_token->next;
	}
}

// words: free可能なcharの2重配列のポインタ
// $から始まる環境変数があれば、展開を行う。
// シングルクォートに囲まれている場合、展開は行わない。
// 先頭から見て、シングルクォートの数が偶数の場合、”シングルクォートに囲まれていない”と判断する。
void	variable_expansion(t_token **token_list, t_list *env_list)
{
	int		j;
	char	*env_var;
	char	**splited_words;
	t_token	*current_token;
	char	*current_quote;
	char	**tmp;
	bool	is_expand;

	if (!token_list || !(*token_list))
		return ;
	current_token = (*token_list);
	is_expand = true;
	while (current_token)
	{
		if (ft_strncmp(current_token->str, "$", 2) == 0)
    {
      current_token = current_token->next;
			continue ;
    }
		else if (ft_strchr(current_token->str, '$'))
		{
			splited_words = ft_multi_split_leave_separator(current_token->str,
					"$\'\"");
			j = -1;
			current_quote = NULL;
			while (splited_words[++j])
			{
				if (current_quote && ft_strchr(splited_words[j],
						current_quote[0]))
				{
          ft_free(current_quote);
					current_quote = NULL;
					is_expand = true;
				}
				else if (!current_quote && (ft_strchr(splited_words[j], '"')
						|| ft_strchr(splited_words[j], '\'')))
				{
					current_quote = ft_strdup(splited_words[j]);
					if (ft_strncmp(current_quote, "\'", 2) == 0)
						is_expand = false;
				}
				if (ft_strncmp(splited_words[j], "$?", 2) == 0 && is_expand)
				{
					tmp = ft_split_by_word_leave_separator(splited_words[j],
							"$?");
					ft_free(tmp[0]);
					tmp[0] = ft_itoa(g_runtime_data.exit_status);
					ft_free(splited_words[j]);
					splited_words[j] = ft_strjoin_all(tmp);
					free_str_array(tmp);
				}
				else if (ft_strchr(splited_words[j], '$') != NULL && is_expand)
				{
					env_var = get_env_value(env_list, &splited_words[j][1]);
					if (env_var)
					{
						ft_free(splited_words[j]);
						splited_words[j] = ft_strdup(env_var);
					}
					else
					{
						ft_free(splited_words[j]);
						splited_words[j] = ft_strdup("");//NULLから変更
					}
				}
			}
			ft_free(current_token->str);
			current_token->str = ft_strjoin_all(splited_words);
			free_str_array(splited_words);
		}
		current_token = current_token->next;
	}
}

void	parse(t_i_mode_vars *i_vars, t_list *env_list)
{
	variable_expansion(&i_vars->token_list, env_list);
}

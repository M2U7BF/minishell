/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/17 13:40:02 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_quotation(char *input_line)
{
	// TODO エラー文を修正。strrchrなどでエラー位置を特定する？
	if (count_chr(input_line, '\'') % 2 == 1)
	{
		ft_dprintf(STDERR_FILENO, "Unclosed quote\n");
		return (1);
	}
	else if (count_chr(input_line, '\"') % 2 == 1)
	{
		ft_dprintf(STDERR_FILENO, "Unclosed quote\n");
		return (1);
	}
	return (0);
}

// （bashの定義と同様に）クォーテーションを削除する。
// 先頭から見て、ダブルクォートの数が偶数の場合、”ダブルクォートに囲まれていない”と判断する。
void	quote_removal(t_token *token)
{
	t_token	*current_token;
	char	**tmp;
	char	*old;
	int		double_quote_count;
	int		i;

	current_token = token;
	double_quote_count = 0;
	i = -1;
	while (current_token)
	{
    if (current_token->type == DELIMITER)
    {
      current_token = current_token->next;  
      continue;
    }
		if (ft_strchr(current_token->str, '\'') != NULL)
		{
			tmp = ft_multi_split_leave_separator(current_token->str, "\'\"");
			while (tmp[++i])
			{
				if (double_quote_count % 2 == 0 && ft_strchr(tmp[i],
						'\'') != NULL)
				{
					old = tmp[i];
					tmp[i] = ft_strtrim(tmp[i], "\'");
					ft_free(old);
				}
				double_quote_count += count_chr(tmp[i], '\"');
			}
			old = current_token->str;
			current_token->str = ft_strjoin_all(tmp);
			free_str_array(tmp);
			free(old);
		}
		i = -1;
		// TODO ダブルクォートの削除
		if (ft_strchr(current_token->str, '\"') != NULL)
		{
			tmp = ft_split_leave_separator(current_token->str, '\"');
			while (tmp[++i])
			{
				old = tmp[i];
				tmp[i] = ft_strtrim(tmp[i], "\"");
				ft_free(old);
			}
			old = current_token->str;
			current_token->str = ft_strjoin_all(tmp);
			free(old);
		}
		current_token = current_token->next;
	}
}

// words: free可能なcharの2重配列のポインタ
// $から始まる環境変数があれば、展開を行う。
// シングルクォートに囲まれている場合、展開は行わない。
// 先頭から見て、シングルクォートの数が偶数の場合、”シングルクォートに囲まれていない”と判断する。
void	variable_expansion(t_token **token_list)
{
	int		j;
	char	*env_var;
	char	**dollar_splited_words;
	t_token	*current_token;
	int		single_quotation_count;
	char	**tmp;

	if (!token_list || !(*token_list))
		return ;
	current_token = (*token_list);
	single_quotation_count = 0;
	while (current_token)
	{
		if (ft_strchr(current_token->str, '$'))
		{
			dollar_splited_words = ft_multi_split_leave_separator(current_token->str,
					"$\'\"");
			j = -1;
			while (dollar_splited_words[++j])
			{
				if (ft_strncmp(dollar_splited_words[j], "$?", 2) == 0
					&& single_quotation_count % 2 == 0)
				{
					tmp = ft_split_by_word_leave_separator(dollar_splited_words[j],
							"$?");
					ft_free(tmp[0]);
					// TODO: 直前のプロセスの最終ステータスを取得する
					tmp[0] = ft_itoa(0);
					ft_free(dollar_splited_words[j]);
					dollar_splited_words[j] = ft_strjoin_all(tmp);
					free_str_array(tmp);
				}
				else if (ft_strchr(dollar_splited_words[j], '$') != NULL
					&& single_quotation_count % 2 == 0)
				{
					env_var = getenv(&dollar_splited_words[j][1]);
					if (env_var)
					{
						ft_free(dollar_splited_words[j]);
						dollar_splited_words[j] = ft_strdup(env_var);
					}
					else
					{
						ft_free(dollar_splited_words[j]);
						dollar_splited_words[j] = ft_strdup("");
					}
				}
				single_quotation_count += count_chr(dollar_splited_words[j],
						'\'');
			}
			ft_free(current_token->str);
			current_token->str = ft_strjoin_all(dollar_splited_words);
			free_str_array(dollar_splited_words);
		}
		current_token = current_token->next;
	}
}

void	parse(t_i_mode_vars *i_vars)
{
	variable_expansion(&i_vars->token_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/06 10:08:26 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_quotation(t_token *token)
{
	t_token	*current_token;
	char	**tmp;
	char	*old;
  int double_quote_count;
  int i;

	current_token = token;
  double_quote_count = 0;
  i = -1;
	while (current_token)
	{
		if (ft_strchr(current_token->str, '\'') != NULL)
		{
			tmp = ft_multi_split_leave_separator(current_token->str, "\'\"");
      while (tmp[++i])
      {
        if (double_quote_count % 2 == 0 && ft_strchr(tmp[i], '\'') != NULL)
        {
          old = tmp[i];
          tmp[i] = ft_strtrim(tmp[i], "\'");
          ft_free(old);
        }
        double_quote_count += count_chr(tmp[i], '\"');
      }
			old = current_token->str;
			current_token->str = ft_strjoin_all(tmp);
			free(old);
		}
		// TODO ダブルクォートの削除
		if (ft_strchr(current_token->str, '\"') != NULL)
		{
			tmp = ft_split(current_token->str, '\"');
			old = current_token->str;
			current_token->str = ft_strjoin_all(tmp);
			free(old);
		}
		current_token = current_token->next;
	}
}

// char	*redirect_expansion(char *s)
// {
// 	//
// }

// char	*pipe_expansion(char *s)
// {
// 	//
// }

// words: free可能なcharの2重配列のポインタ
// $から始まる環境変数があれば、展開を行う。
// 細かい挙動はbashに従う。
void	variable_expansion(t_token **token_list)
{
	int		i;
	int		j;
	char	*env_var;
	char	**dollar_splited_words;
	t_token	*current_token;
	int		single_quotation_count;

	if (!token_list || !(*token_list))
		return ;
	i = -1;
	current_token = (*token_list);
	single_quotation_count = 0;
	while (current_token)
	{
		if (ft_strchr(current_token->str, '$'))
		{
			dollar_splited_words = ft_multi_split_leave_separator(current_token->str,
					"$'");
			j = -1;
			while (dollar_splited_words[++j])
			{
				if (ft_strchr(dollar_splited_words[j], '$') != NULL
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
	// TODO プロセス数を判定後、下記を実施。
	// redirect_expansion(vars->input_line);
	// pipe_expansion(vars->input_line);
}

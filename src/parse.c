/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/05 15:29:53 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// TODO tokenizerで置換予定
char	**word_segmentation(char *input)
{
	char	*ifs;

	ifs = getenv("IFS");
	return (ft_split_multiple_separators(input, ifs));
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
  t_token *current_token;

	if (!token_list || !(*token_list))
		return ;
	i = -1;
  current_token = (*token_list);
	while (current_token)
	{
		if (ft_strchr(current_token->str, '$'))
		{
			dollar_splited_words = ft_split_leave_separator(current_token->str, '$');
			j = -1;
			while (dollar_splited_words[++j])
			{
				if (ft_strchr(dollar_splited_words[j], '$') != NULL)
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

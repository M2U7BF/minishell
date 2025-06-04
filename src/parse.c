/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/04 11:41:50 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**word_segmentation(char *input)
{
	// TODO getenv("IFS")
	return (ft_split(input, ' '));
}

// char	*redirect_expansion(char *s)
// {
// 	//
// }

// char	*pipe_expansion(char *s)
// {
// 	//
// }

// wordsの要素はmalloc可能な文字列
// $を探す
// 環境変数と一致するか検索する
// 一致し次第、置換し、文字列を更新する
// ※戻り値以外のメモリは全て解放する。
void	variable_expansion(char ***words)
{
	int		i;
	int		j;
	char	*env_var;
	char	**dollar_splited_words;

	i = -1;
	while ((*words)[++i])
	{
		if (ft_strchr((*words)[i], '$'))
		{
			dollar_splited_words = ft_split_leave_separator((*words)[i], '$');
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
			ft_free((*words)[i]);
			(*words)[i] = ft_strjoin_all(dollar_splited_words);
			free_str_array(dollar_splited_words);
		}
	}
}

void	parse(t_i_mode_vars *i_vars)
{
	i_vars->words = word_segmentation(i_vars->input_line);
	variable_expansion(&i_vars->words);
	// TODO プロセス数を判定後、下記を実施。
	// redirect_expansion(vars->input_line);
	// pipe_expansion(vars->input_line);
}

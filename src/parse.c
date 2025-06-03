/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/03 12:53:43 by kkamei           ###   ########.fr       */
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

// $を探す
// 環境変数と一致するか検索する
// 一致し次第、置換し、文字列を更新する
// ※戻り値以外のメモリは全て解放する。
void	variable_expansion(char ***words)
{
	int		i;
	int		j;
	char	*env_var;
	char	*front_str;

	i = -1;
	while ((*words)[++i])
	{
		j = -1;
		while ((*words)[i][++j])
		{
			if ((*words)[i][j] == '$')
			{
				env_var = getenv(&(*words)[i][j + 1]);
				if (env_var)
				{
					if (j > 0)
					{
						front_str = malloc(sizeof(char) * j);
						ft_strlcpy(front_str, (*words)[i], j);
					}
					else
						front_str = "";
					free((*words)[i]);
					(*words)[i] = ft_strjoin(front_str, env_var);
				}
			}
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

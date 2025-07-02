/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 17:40:01 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_question_mark(char **s)
{
	char	**tmp;

	tmp = ft_split_by_word_keep_sep(*s, "$?");
	ft_free((void **)&tmp[0]);
	tmp[0] = ft_itoa(g_vars.exit_status);
	ft_free((void **)s);
	*s = ft_strjoin_all(tmp);
	free_str_array(tmp);
}

void	expand_variable(char **s)
{
	char	*env_var;

	env_var = getenv(&(*s)[1]);
	if (env_var)
	{
		ft_free((void **)s);
		*s = ft_strdup(env_var);
	}
	else
		ft_free((void **)s);
}

// words: free可能なcharの2重配列のポインタ
// $から始まる環境変数があれば、展開を行う。
// シングルクォートに囲まれている場合、展開は行わない。
void	variable_expansion(t_token **token_list)
{
	int		j;
	char	**splited_words;
	t_token	*current_token;
	char	*current_quote;
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
					ft_free((void **)&current_quote);
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
					expand_question_mark(&splited_words[j]);
				else if (ft_strchr(splited_words[j], '$') != NULL && is_expand)
					expand_variable(&splited_words[j]);
			}
			ft_free((void **)&current_token->str);
			current_token->str = ft_strjoin_all(splited_words);
			free_str_array(splited_words);
		}
		current_token = current_token->next;
	}
}

void	parse(t_i_mode_vars *i_vars)
{
	if (is_syntax_error(i_vars->token_list))
	{
		ft_dprintf(STDERR_FILENO, "syntax_error\n");
		destroy_i_vars(i_vars);
		exit(EXIT_SYNTAX_ERROR);
	}
	variable_expansion(&i_vars->token_list);
}

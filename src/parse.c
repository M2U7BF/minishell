/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/07 13:37:32 by kkamei           ###   ########.fr       */
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
	free_str_array(&tmp);
}

void	expand_variable(char **s)
{
	char	*env_var;

	env_var = get_env_value(g_vars.env_list, *s + 1);
	if (env_var)
	{
		ft_free((void **)s);
		*s = ft_strdup(env_var);
	}
	else
		ft_free((void **)s);
}

void	inner_process(char *current_quote, char **s, bool *is_expand)
{
	if (*current_quote && ft_strchr(*s, *current_quote))
	{
		*current_quote = '\0';
		*is_expand = true;
	}
	else if (!*current_quote && (*s[0] == '"' || *s[0] == '\''))
	{
		*current_quote = *s[0];
		if (*current_quote == '\'')
			*is_expand = false;
	}
	if (*s[0] == '$')
	{
		if (is_str_equal(*s, "$", true))
			return ;
		if (*is_expand)
		{
			if (is_str_equal(*s, "$?", false))
				expand_question_mark(s);
			else
				expand_variable(s);
		}
	}
}

// words: free可能なcharの2重配列のポインタ
// $から始まる環境変数があれば、展開を行う。
// シングルクォートに囲まれている場合、展開は行わない。
void	variable_expansion(t_token **token_list)
{
	int		j;
	char	**splited_words;
	t_token	*cur_tok;
	char	cur_quote;
	bool	is_expand;

	if (!token_list || !(*token_list))
		return ;
	cur_tok = (*token_list);
	is_expand = true;
	while (cur_tok)
	{
		if (!is_str_equal("$", cur_tok->str, true) && ft_strchr(cur_tok->str,
				'$'))
		{
			splited_words = ft_multi_split_keep_sep(cur_tok->str, "$'\" \t");
			j = -1;
			cur_quote = '\0';
			while (splited_words[++j])
				inner_process(&cur_quote, &splited_words[j], &is_expand);
			ft_free((void **)&cur_tok->str);
			cur_tok->str = ft_strjoin_all(splited_words);
			free_str_array(&splited_words);
		}
		cur_tok = cur_tok->next;
	}
}

void	parse(t_i_mode_vars *i_vars)
{
	if (is_syntax_error(i_vars->token_list))
	{
		ft_dprintf(STDERR_FILENO, "syntax_error\n");
		destroy_i_vars(i_vars);
		free_env_list(&g_vars.env_list);
		exit(EXIT_SYNTAX_ERROR);
	}
	variable_expansion(&i_vars->token_list);
	quote_removal(i_vars->token_list);
}

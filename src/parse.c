/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/28 10:44:18 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand(char *current_quote, char **s, bool *is_expand)
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
		if (is_s_eq(*s, "$", true))
			return ;
		if (*is_expand)
		{
			if (is_s_eq(*s, "$?", false))
				expand_question_mark(s);
			else
				expand_variable(s);
		}
	}
}

void	inner_process(t_token **cur_tok, bool *is_expand, t_token **token_list,
		bool is_heredoc)
{
	int		j;
	char	**splited_words;
	char	cur_quote;
	t_token	*old;

	splited_words = ft_multi_split_keep_sep((*cur_tok)->str, "$'\" \t");
	j = -1;
	cur_quote = '\0';
	while (splited_words[++j])
		expand(&cur_quote, &splited_words[j], is_expand);
	ft_free((void **)&(*cur_tok)->str);
	(*cur_tok)->str = ft_strjoin_all(splited_words);
	free_str_array(&splited_words);
	if (!is_heredoc && !(*cur_tok)->str[0])
	{
		old = *cur_tok;
		*cur_tok = (*cur_tok)->next;
		del_token(token_list, old);
	}
}

// words: A pointer to a double array of char that can be freed
// Expands environment variables starting with $
// No expansion is performed if enclosed in single quotes
void	variable_expansion(t_token **token_list, bool is_heredoc)
{
	t_token	*cur_tok;
	bool	is_expand;

	if (!token_list || !(*token_list))
		return ;
	cur_tok = (*token_list);
	is_expand = true;
	while (cur_tok)
	{
		if (!is_s_eq("$", cur_tok->str, true) && ft_strchr(cur_tok->str, '$'))
			inner_process(&cur_tok, &is_expand, token_list, is_heredoc);
		if (cur_tok)
			cur_tok = cur_tok->next;
	}
}

int	parse(t_i_mode_vars *i_vars)
{
	t_list	*env_list;

	env_list = access_env_list(false, NULL);
	if (is_syntax_error(i_vars->token_list))
	{
		ft_dprintf(STDERR_FILENO, "syntax_error\n");
		destroy_i_vars(i_vars);
		free_env_list(&env_list);
		access_exit_status(true, EXIT_SYNTAX_ERROR);
		return (-1);
	}
	variable_expansion(&i_vars->token_list, false);
	if (quote_removal(i_vars->token_list) == -1)
		return (-1);
	return (0);
}

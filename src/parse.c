/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 16:55:14 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_question_mark(char **s)
{
	char	**tmp;

	tmp = ft_split_by_word_keep_sep(*s, "$?");
	ft_free((void **)&tmp[0]);
	tmp[0] = ft_itoa(access_exit_status(false, 0));
	ft_free((void **)s);
	*s = ft_strjoin_all(tmp);
	free_str_array(&tmp);
}

void	expand_variable(char **s)
{
	char	*var;
	char	*name;
	int		end;
	char	*new;

	end = 0;
	name = get_var_name(*s + 1, &end);
	var = get_env_value(access_env_list(false, NULL), name);
	new = NULL;
	if (!name[0])
		new = ft_strdup(*s);
	else if (var)
	{
		new = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(*s) - end + 1));
		ft_strlcpy(new, var, ft_strlen(var) + 1);
		ft_strlcpy(new + ft_strlen(var), *s + end + 1, ft_strlen(*s) - end + 1);
	}
	else
	{
		new = malloc(sizeof(char) * (ft_strlen(*s) - end + 1));
		ft_strlcpy(new, *s + end + 1, ft_strlen(*s) - end + 1);
	}
	ft_free((void **)s);
	*s = new;
	ft_free((void **)&name);
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

// words: A pointer to a double array of char that can be freed
// Expands environment variables starting with $
// No expansion is performed if enclosed in single quotes
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
		if (!is_s_eq("$", cur_tok->str, true) && ft_strchr(cur_tok->str, '$'))
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

int	parse(t_i_mode_vars *i_vars)
{
  t_list *env_list;

  env_list = access_env_list(false, NULL);
	if (is_syntax_error(i_vars->token_list))
	{
		ft_dprintf(STDERR_FILENO, "syntax_error\n");
		destroy_i_vars(i_vars);
		free_env_list(&env_list);
		access_exit_status(true, EXIT_SYNTAX_ERROR);
		return (-1);
	}
	variable_expansion(&i_vars->token_list);
	if (quote_removal(i_vars->token_list) == -1)
		return (-1);
	return (0);
}

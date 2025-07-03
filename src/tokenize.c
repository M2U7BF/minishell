/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:35:00 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 10:16:22 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	tokenize_error(void)
{
	ft_dprintf(STDERR_FILENO, "token error\n");
}

char	**split_line(char *input_line)
{
	char		**w;
	static char	*redirection_list[] = REDIRECTION_LIST;
	static char	*blank_list[] = BLANK_LIST;
	static char	*quotation_list[] = QUOTATION_LIST;

	if (!input_line)
		return (NULL);
	w = ft_multi_split_keep_sep(input_line, DEFAULT_BLANK);
	w = ft_multi_splitarr_by_word_leave_separator(w, redirection_list);
	w = ft_multi_splitarr_by_word_leave_separator(w, blank_list);
	w = ft_multi_splitarr_by_word_leave_separator(w, quotation_list);
	w = ft_splitarr_by_word_leave_separator(w, "|");
	return (w);
}

int	determine_token_type(char *s)
{
	t_token_type	type;

	type = ERROR;
	if (ft_strncmp(s, "|", 2) == 0)
		type = PIPE;
	else if (is_control_operator(s))
		type = CONTROL_OPERATOR;
	else if (is_reserved_word(s))
		type = RESERVED_WORD;
	else if (is_redirection(s))
		type = REDIRECTION;
	else if (is_word(s))
		type = WORD;
	else if (is_blank(s))
		type = BLANK;
	else
		tokenize_error();
	return (type);
}

void	process_token_list(t_token **token_list)
{
	t_token	*current;

	process_str_quoted(*token_list);
	*token_list = join_tokens(*token_list);
	remove_blank(*token_list);
	current = *token_list;
	while (current)
	{
		if (current->type == REDIRECTION && ft_strncmp(current->str, "<<",
				3) == 0)
		{
			if (current->next && current->next->type == WORD)
				current->next->type = DELIM;
		}
		current = current->next;
	}
}

t_token	*tokenize(char *input_line)
{
	char			**w;
	int				i;
	t_token			*token_list;
	t_token_type	type;

	w = split_line(input_line);
	if (!w)
		return (NULL);
	i = -1;
	token_list = NULL;
	while (w[++i])
	{
		type = determine_token_type(w[i]);
		append_token(&token_list, create_token(w[i], type));
	}
	ft_free((void **)&w);
	process_token_list(&token_list);
	return (token_list);
}

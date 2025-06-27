/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:35:00 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 12:45:53 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_metacharacter(char *s)
{
	int			i;
	static char	*chars[] = {"|", "&", ";", "(", ")", "<", ">", " ", "\t", "\v"};
	static int	len = sizeof(chars) / sizeof(chars[0]);

	if (!s)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

bool	is_word(char *s)
{
	return (*s && !is_metacharacter(s));
}

bool	is_control_operator(char *s)
{
	int			i;
	static char	*opes[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "|&",
			"\n"};
	static int	len = sizeof(opes) / sizeof(opes[0]);

	if (!s)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, opes[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

bool	is_reserved_word(char *s)
{
	int			i;
	static char	*words[] = {"!", "case", "do", "done", "elif", "else", "esac",
			"fi", "for", "function", "if", "in", "select", "then", "until",
			"while", "{", "}", "time", "[[", "]]"};
	static int	len = sizeof(words) / sizeof(words[0]);

	if (!s)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, words[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

bool	is_redirection(char *s)
{
	int			i;
	static char	*chars[] = REDIRECTION_LIST;

	if (!s)
		return (false);
	i = -1;
	while (chars[++i])
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

static void	tokenize_error(void)
{
	ft_dprintf(STDERR_FILENO, "token error\n");
}

t_token	*tokenize(char *input_line)
{
	char			**w;
	char			**old_w;
	int				i;
	t_token			*token_list;
	t_token			*current;
	t_token_type	type;
	static char		*redirection_list[] = REDIRECTION_LIST;
	static char		*blank_list[] = BLANK_LIST;
	static char		*quotation_list[] = QUOTATION_LIST;

	if (!input_line)
		return (NULL);
	w = ft_multi_split_leave_separator(input_line, DEFAULT_BLANK);
	old_w = w;
	w = ft_multi_splitarr_by_word_leave_separator(w, redirection_list);
	free_str_array(old_w);
	old_w = w;
	w = ft_multi_splitarr_by_word_leave_separator(w, blank_list);
	free_str_array(old_w);
	old_w = w;
	w = ft_multi_splitarr_by_word_leave_separator(w, quotation_list);
	free_str_array(old_w);
	old_w = w;
	w = ft_splitarr_by_word_leave_separator(w, "|");
	free_str_array(old_w);
	if (!w)
		return (NULL);
	i = -1;
	while (w[++i])
	{
		if (ft_strncmp(w[i], "|", 2) == 0)
			type = PIPE;
		else if (is_control_operator(w[i]))
			type = CONTROL_OPERATOR;
		else if (is_reserved_word(w[i]))
			type = RESERVED_WORD;
		else if (is_redirection(w[i]))
			type = REDIRECTION;
		else if (is_word(w[i]))
			type = WORD;
		else if (is_include(w[i], blank_list))
			type = BLANK;
		else
		{
			printf("w[i]: %s\n", w[i]);
			tokenize_error();
		}
		if (i == 0)
			token_list = create_token(w[i], type);
		else
			append_token(&token_list, create_token(w[i], type));
	}
	ft_free(w);
	token_list = process_single_quote(token_list);
	token_list = process_double_quote(token_list);
	token_list = join_tokens(token_list);
	process_blank(token_list);
	token_list = join_tokens(token_list);
	remove_blank(token_list);
	current = token_list;
	while (current)
	{
		if (current->type == REDIRECTION && ft_strncmp(current->str, "<<",
				3) == 0)
		{
			if (current->next && current->next->type == WORD)
				current->next->type = DELIMITER;
		}
		current = current->next;
	}
	return (token_list);
}

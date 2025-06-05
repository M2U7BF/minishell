/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:31:55 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/05 10:39:13 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	debug_put_token_list(t_token *token_list)
{
	t_token	*current_token;
	int		i;

	if (!token_list)
	{
		printf("(null)\n");
		return ;
	}
	current_token = token_list;
	i = 0;
	while (current_token)
	{
		printf("[%d] str:\"%s\", type:%d\n", i, current_token->str,
			current_token->type);
		current_token = current_token->next;
		i++;
	}
}

void	free_token_list(t_token *token_list)
{
	t_token	*current_token;
	t_token	*tmp;

	if (!token_list)
		return ;
	current_token = token_list;
	while (current_token)
	{
		tmp = current_token;
		current_token = current_token->next;
		free(tmp->str);
		free(tmp);
	}
}

t_token	*create_token(char *str, t_token_type type)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->next = NULL;
	token->str = str;
	token->type = type;
	return (token);
}

void	append_token(t_token **token_list, char *str, t_token_type type)
{
	t_token	*current_token;

	if (!token_list || !(*token_list) || !str)
		return ;
	current_token = (*token_list);
	while (current_token)
	{
		if (!current_token->next)
		{
			current_token->next = create_token(str, type);
			break ;
		}
		current_token = current_token->next;
	}
}

int	is_control_operator(char *s)
{
	int			i;
	static char	*opes[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "|&",
			"\n"};
	static int	len = sizeof(opes) / sizeof(opes[0]);

	if (!s)
		return (0);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, opes[i], ft_strlen(s) + 1) == 0)
			return (1);
	}
	return (0);
}

int	is_reserved_word(char *s)
{
	int			i;
	static char	*words[] = {"!", "case", "do", "done", "elif", "else", "esac",
			"fi", "for", "function", "if", "in", "select", "then", "until",
			"while", "{", "}", "time", "[[", "]]"};
	static int	len = sizeof(words) / sizeof(words[0]);

	if (!s)
		return (0);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, words[i], ft_strlen(s) + 1) == 0)
			return (1);
	}
	return (0);
}

t_token	*tokenize(char *input_line)
{
	char			**w;
	int				i;
	t_token			*token_list;
	t_token_type	type;

	if (!input_line)
		return (NULL);
	w = ft_split_multiple_separators(input_line, DEFAULT_BLANK);
	// TODO NULLの場合の処理必要？
	i = -1;
	while (w[++i])
	{
		if (is_control_operator(w[i]))
			type = CONTROL_OPERATOR;
		else if (is_reserved_word(w[i]))
			type = RESERVED_WORD;
		else
			type = WORD;
		if (i == 0)
			token_list = create_token(w[i], type);
		else
			append_token(&token_list, w[i], type);
	}
	ft_free(w);
	return (token_list);
}

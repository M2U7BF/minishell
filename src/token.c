/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:31:55 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/06 11:47:36 by kkamei           ###   ########.fr       */
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

int	debug_put_token_list_compare(t_token *t, t_token *t_e)
{
	int		i;
	int		is_not_all_equal;
	int		is_different;
	t_token	*current_t;
	t_token	*current_t_e;

	if (t == NULL)
		return (-1);
	i = 0;
	is_not_all_equal = 0;
	current_t = t;
	current_t_e = t_e;
	while (current_t)
	{
		is_different = strncmp(current_t->str, current_t_e->str,
				strlen(current_t_e->str) + 1);
		if (is_different != 0)
			is_not_all_equal = 1;
		if (is_different)
			printf("[%d] [❌] result:\"%s\", expected:\"%s\"\n", i,
				current_t->str, current_t_e->str);
		else
			printf("[%d] [⭕] result:\"%s\", expected:\"%s\"\n", i,
				current_t->str, current_t_e->str);
		current_t = current_t->next;
		current_t_e = current_t_e->next;
	}
	if (is_not_all_equal == 0)
		printf("すべて同じ 💎\n");
	else
		printf("すべて同じではない 🔥\n");
	return (is_not_all_equal);
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

int	is_metacharacter(char *s)
{
	int			i;
	static char	*chars[] = {"|", "&", ";", "(", ")", "<", ">", " ", "\t", "\v"};
	static int	len = sizeof(chars) / sizeof(chars[0]);

	if (!s)
		return (0);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
			return (1);
	}
	return (0);
}

int	is_word(char *s)
{
	return (*s && !is_metacharacter(s));
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

void	tokenize_error(void)
{
	ft_dprintf(STDERR_FILENO, "token error\n");
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
		else if (is_word(w[i]))
			type = WORD;
		else
			tokenize_error();
		if (i == 0)
			token_list = create_token(w[i], type);
		else
			append_token(&token_list, w[i], type);
	}
	ft_free(w);
	return (token_list);
}

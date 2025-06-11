/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:31:55 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/11 14:37:23 by kkamei           ###   ########.fr       */
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

void	append_token(t_token **token_list, t_token *token)
{
	t_token	*current_token;

	if (!token_list || !(*token_list) || !token)
		return ;
	current_token = (*token_list);
	while (current_token)
	{
		if (!current_token->next)
		{
			current_token->next = token;
			break ;
		}
		current_token = current_token->next;
	}
}

t_token	*token_dup(t_token *token)
{
	if (!token)
		return (NULL);
	return (create_token(ft_strdup(token->str), token->type));
}

int	token_list_len(t_token *token_list)
{
	t_token	*current_token;
	int		i;

	if (!token_list)
		return (0);
	current_token = token_list;
	i = 0;
	while (current_token)
	{
		i++;
		current_token = current_token->next;
	}
	return (i);
}

char	**tokens_to_arr(t_token *token_list)
{
	int		i;
	int		len;
	char	**arr;
	t_token	*current_token;

	if (!token_list)
		return (0);
	len = token_list_len(token_list);
	arr = malloc(sizeof(char *) * (len + 1));
	current_token = token_list;
	i = 0;
	while (i < len && current_token)
	{
		arr[i] = ft_strdup(current_token->str);
		current_token = current_token->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
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

int	is_variable(char *s)
{
	if (!s)
		return (0);
	return (s[0] == '$');
}

int	is_redirection(char *s)
{
	int			i;
	static char	*chars[] = REDIRECTION_LIST;

	if (!s)
		return (0);
	i = -1;
	while (chars[++i])
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
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
	static char		*redirection_list[] = REDIRECTION_LIST;
	static char		*blank_list[] = BLANK_LIST;
	static char		*quotation_list[] = QUOTATION_LIST;
	// static char		*dollar[] = {"$", NULL};

	if (!input_line)
		return (NULL);
	w = ft_multi_split_leave_separator(input_line, DEFAULT_BLANK);
	w = ft_multi_splitarr_by_word_leave_separator(w, redirection_list);
	w = ft_multi_splitarr_by_word_leave_separator(w, blank_list);
	w = ft_multi_splitarr_by_word_leave_separator(w, quotation_list);
  w = ft_splitarr_leave_separator(w, '$');
  // TODO: クォートの処理（クォートでエスケープされる文字はすべて結合する）
  // TODO: blankの削除
  w = remove_elem(w, blank_list);
	// printf("w:\n");
	// put_strarr(w);
	// TODO NULLの場合の処理必要？
	i = -1;
	while (w[++i])
	{
		if (is_control_operator(w[i]))
			type = CONTROL_OPERATOR;
		else if (is_reserved_word(w[i]))
			type = RESERVED_WORD;
		else if (is_variable(w[i]))
			type = VARIABLE;
		else if (is_redirection(w[i]))
			type = REDIRECTION;
		else if (is_word(w[i]))
		{
			type = WORD;
		}
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
	return (token_list);
}

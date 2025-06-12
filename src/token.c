/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:31:55 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/12 11:23:26 by kkamei           ###   ########.fr       */
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

// int	is_variable(char *s)
// {
// 	if (!s)
// 		return (0);
// 	return (s[0] == '$');
// }

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

void	del_token(t_token **token_list, t_token *token)
{
	t_token	*current;
	t_token	*prev;

	current = *token_list;
	prev = NULL;
	if (!token_list || !*token_list || !token)
		return ;
	if (*token_list == token)
		*token_list = token->next;
	else
	{
		while (current && current != token)
		{
			prev = current;
			current = current->next;
		}
		if (current == token)
			prev->next = token->next;
	}
	ft_free(token->str);
	free(token);
}

void	insert_token(t_token **first_token, t_token *token, t_token *add)
{
	t_token	*next_token;

	if (!add)
		return ;
	if (!token)
	{
		add->next = *first_token;
		*first_token = add;
	}
	else
	{
		next_token = token->next;
		token->next = add;
		add->next = next_token;
	}
}

t_token	*get_prev_token(t_token **token_list, t_token *token)
{
	t_token	*current_token;
	t_token	*prev;

	if (*token_list == token)
		return (NULL);
	current_token = *token_list;
	prev = NULL;
	while (current_token)
	{
		if (current_token == token)
			return (prev);
		prev = current_token;
		current_token = current_token->next;
	}
	return (NULL);
}

// シングルクォート内の文字列はすべて1つのWORDとして結合する
void	process_single_quote(t_token *token_list)
{
	t_list	*tmp;
	t_token	*current_token;
	t_token	*old;
	int		single_quote_count;
	char	**tmp_arr;
	char	*tmp_str;

	current_token = token_list;
	single_quote_count = 0;
	tmp = NULL;
	while (current_token)
	{
		if (ft_strncmp(current_token->str, "\'", 2) == 0)
		{
			// 終端のシングルクォートの結合
			if (single_quote_count > 0)
			{
				old = current_token;
				current_token = get_prev_token(&token_list, old);
				ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
				del_token(&token_list, old);
				tmp_arr = lst_to_str_arr(tmp);
				tmp_str = ft_strjoin_all(tmp_arr);
				free_str_array(tmp_arr);
				ft_lstclear(&tmp, del_content);
				insert_token(&token_list, current_token, create_token(tmp_str,
						WORD));
			}
			single_quote_count++;
		}
		// 先端のシングルクォート、シングルクォートで囲まれている文字列の結合
		if (single_quote_count % 2 == 1)
		{
			old = current_token;
			current_token = get_prev_token(&token_list, old);
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
			del_token(&token_list, old);
		}
		current_token = current_token->next;
	}
}

// TODO: ダブルクォート内の文字列は展開される文字列だけ特別扱いして、それ以外は結合
void	process_double_quote(t_token *token_list)
{
	t_list	*tmp;
	t_token	*current_token;
	t_token	*old;
	int		double_quote_count;
	char	**tmp_arr;
	char	*tmp_str;

	current_token = token_list;
	double_quote_count = 0;
	tmp = NULL;
	while (current_token)
	{
		if (ft_strncmp(current_token->str, "\"", 2) == 0)
		{
			// 終端のシングルクォートの結合
			if (double_quote_count > 0)
			{
				old = current_token;
				current_token = get_prev_token(&token_list, old);
				ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
				del_token(&token_list, old);
				tmp_arr = lst_to_str_arr(tmp);
				tmp_str = ft_strjoin_all(tmp_arr);
				free_str_array(tmp_arr);
				ft_lstclear(&tmp, del_content);
				insert_token(&token_list, current_token, create_token(tmp_str,
						WORD));
			}
			double_quote_count++;
		}
		// 先端のシングルクォート、シングルクォートで囲まれている文字列の結合
		if (double_quote_count % 2 == 1)
		{
			old = current_token;
			current_token = get_prev_token(&token_list, old);
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
			del_token(&token_list, old);
		}
		current_token = current_token->next;
	}
}

// BLANK, REDIRECTION, を挟まずに隣り合う文字列を結合する。
t_token	*join_tokens(t_token *token_list)
{
	t_list	*tmp;
	t_token	*current;
	t_token	*prev;
	t_token	*old;
	t_token	*old_prev;
	char	**tmp_arr;
	char	*tmp_str;

	current = token_list;
	prev = NULL;
	tmp = NULL;
	while (current)
	{
		if (current != NULL && prev != NULL && prev->type == WORD
			&& current->type == WORD)
		{
			old_prev = prev;
			old = current;
			current = get_prev_token(&token_list, old_prev);
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old_prev->str)));
			ft_lstadd_back(&tmp, ft_lstnew((void *)ft_strdup(old->str)));
			del_token(&token_list, old);
			del_token(&token_list, old_prev);
			tmp_arr = lst_to_str_arr(tmp);
			tmp_str = ft_strjoin_all(tmp_arr);
			free_str_array(tmp_arr);
			ft_lstclear(&tmp, del_content);
			insert_token(&token_list, current, create_token(tmp_str, WORD));
		}
		prev = current;
		if (!current)
			current = token_list;
		else
			current = current->next;
	}
	return (token_list);
}

void	remove_blank(t_token *token_list)
{
	t_token	*current_token;
	t_token	*old;

	current_token = token_list;
	while (current_token)
	{
		if (current_token->type == BLANK)
		{
			old = current_token;
			current_token = current_token->next;
			del_token(&token_list, old);
		}
		else
			current_token = current_token->next;
	}
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

	if (!input_line)
		return (NULL);
	w = ft_multi_split_leave_separator(input_line, DEFAULT_BLANK);
	w = ft_multi_splitarr_by_word_leave_separator(w, redirection_list);
	w = ft_multi_splitarr_by_word_leave_separator(w, blank_list);
	w = ft_multi_splitarr_by_word_leave_separator(w, quotation_list);
	// w = ft_splitarr_leave_separator(w, '$');
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
	process_single_quote(token_list);
  process_double_quote(token_list);
	join_tokens(token_list);
	remove_blank(token_list);
	return (token_list);
}

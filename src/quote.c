/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:40:49 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 12:41:09 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_quotation_error(char *input_line)
{
	if (count_chr(input_line, '\'') % 2 == 1)
	{
		ft_dprintf(STDERR_FILENO, "Unclosed quote\n");
		return (true);
	}
	else if (count_chr(input_line, '\"') % 2 == 1)
	{
		ft_dprintf(STDERR_FILENO, "Unclosed quote\n");
		return (true);
	}
	return (false);
}

static char	*remove_quotes(char *s)
{
	int		i;
	int		j;
	char	current_quote;
	char	*result;

	current_quote = '\0';
	i = -1;
	j = 0;
	result = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!result)
		return (NULL);
	while (s[++i])
	{
		if (!current_quote && is_quote(s[i]))
			current_quote = s[i];
		else if (current_quote && current_quote == s[i])
			current_quote = '\0';
		else
			result[j++] = s[i];
	}
	result[j] = '\0';
	return (result);
}

// （bashの定義と同様に）クォーテーションを削除する。
void	quote_removal(t_token *token)
{
	t_token	*current;
	char	*new;

	current = token;
	while (current && current->str)
	{
		if (current->type == WORD && (ft_strchr(current->str, '\'') != NULL
				|| ft_strchr(current->str, '\"') != NULL))
		{
			new = remove_quotes(current->str);
			ft_free((void **)&current->str);
			current->str = new;
		}
		current = current->next;
	}
}

bool	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

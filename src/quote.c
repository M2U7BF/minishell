/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:40:49 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/10 18:03:16 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*remove_quotes(char *s, int *status)
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
	if (current_quote)
		*status = -1;
	result[j] = '\0';
	return (result);
}

// （bashの定義と同様に）クォーテーションを削除する。
int	quote_removal(t_token *token)
{
	t_token	*current;
	char	*new;
	int		status;

	current = token;
	status = 0;
	while (current && current->str)
	{
		if (current->type == WORD && (ft_strchr(current->str, '\'') != NULL
				|| ft_strchr(current->str, '\"') != NULL))
		{
			new = remove_quotes(current->str, &status);
			if (status == -1)
			{
				ft_dprintf(STDERR_FILENO, "%s\n", ERR_QUOTE_1);
				return (-1);
			}
			ft_free((void **)&current->str);
			current->str = new;
		}
		current = current->next;
	}
	return (0);
}

bool	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

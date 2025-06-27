/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blank.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:36:48 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 12:37:12 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	process_blank(t_token *token_list)
{
	t_token		*current_token;
	char		*current_quote;
	static char	*quote[] = {"\"", "\'", NULL};
	char		**tmp;
	int			i;

	current_quote = NULL;
	current_token = token_list;
	while (current_token)
	{
		if (current_token->type == BLANK && current_quote)
		{
			current_token->type = WORD;
		}
		else if (current_token->type == WORD)
		{
			tmp = ft_multi_split_by_word_leave_separator(current_token->str,
					quote);
			i = -1;
			while (tmp && tmp[++i])
			{
				if (!current_quote && (ft_strncmp("\"", tmp[i], 2) == 0
							|| ft_strncmp("\'", tmp[i], 2) == 0))
				{
					current_quote = ft_strdup(tmp[i]);
					continue ;
				}
				if (current_quote && ft_strncmp(current_quote, tmp[i], 2) == 0)
				{
					ft_free(current_quote);
					current_quote = NULL;
					continue ;
				}
			}
			free_str_array(tmp);
			current_token = current_token->next;
		}
		else
			current_token = current_token->next;
	}
}

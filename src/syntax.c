/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:24:48 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/17 13:38:52 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_redirection_syntax(t_token *token_list)
{
	t_token	*current_token;

	// redirection の後ろに制御文字があったらエラー
	current_token = token_list;
	while (current_token)
	{
		if (current_token->type == REDIRECTION && (current_token->next == NULL
				|| (current_token->next->type != WORD && current_token->next->type != DELIMITER)))
			return (-1);
		current_token = current_token->next;
	}
	return (0);
}

int	check_syntax_error(t_token *token_list)
{
	if (!token_list)
		return (-1);
	if (check_redirection_syntax(token_list) != 0)
		return (-1);
	return (0);
}

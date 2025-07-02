/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_unit_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 16:58:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 17:00:31 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	proc_len(t_proc_unit *proc_list)
{
	int			i;
	t_proc_unit	*current;

	if (!proc_list)
		return (0);
	i = 0;
	current = proc_list;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

t_proc_unit	*process_division(t_token *token_list)
{
	t_token		*tok;
	t_proc_unit	*result;
	t_proc_unit	*proc;

	if (!token_list)
		return (NULL);
	proc = new_proc(tok_dup(token_list), CMD, STDIN_FILENO, STDOUT_FILENO);
	result = proc;
	tok = token_list->next;
	while (tok)
	{
		if (tok->type == PIPE)
		{
			tok = tok->next;
			proc->next = new_proc(tok_dup(tok), PIPE_LINE, STDIN_FILENO,
					STDOUT_FILENO);
			proc = proc->next;
		}
		else
			append_token(&proc->args, tok_dup(tok));
		tok = tok->next;
	}
	return (result);
}

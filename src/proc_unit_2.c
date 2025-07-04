/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_unit_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 16:58:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/04 15:58:49 by atashiro         ###   ########.fr       */
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

void	set_argv(t_proc_unit *current_proc)
{
	current_proc->argv = tokens_to_arr(current_proc->args);
	current_proc->argv = trim_redirection(&current_proc->argv);
	if (!current_proc->argv)
		exit(EXIT_SUCCESS);
	// atashiro-----------------------
	if (is_builtin(current_proc->argv[0]))
		exit(exec_builtin(current_proc->argv));
	//-------------
	g_vars.exit_status = get_command_path(&current_proc->argv[0]);
	if (g_vars.exit_status != 0)
	{
		handle_error(g_vars.exit_status, current_proc->argv[0]);
		exit(g_vars.exit_status);
	}
}

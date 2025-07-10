/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_unit_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 16:58:07 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/10 14:41:05 by kkamei           ###   ########.fr       */
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

t_proc_unit	*process_division(t_i_mode_vars *i_vars)
{
	t_token		*t;
	t_proc_unit	*result;
	t_proc_unit	*p;

	p = new_proc(tok_dup(i_vars->token_list), CMD, STDIN_FILENO, STDOUT_FILENO);
	result = p;
	t = i_vars->token_list->next;
	while (t)
	{
		if (t->type == PIPE)
		{
			t = t->next;
			p->status = set_argv(p);
			p->next = new_proc(tok_dup(t), PLINE, STDIN_FILENO, STDOUT_FILENO);
			p = p->next;
		}
		else
			append_token(&p->args, tok_dup(t));
		t = t->next;
	}
	p->status = set_argv(p);
	if (!result->next && result->argv && is_builtin(result->argv[0]))
		result->type = ONLY_PARENT;
	return (update_proc(i_vars, result), result);
}

int	set_argv(t_proc_unit *current_proc)
{
	current_proc->argv = tokens_to_arr(current_proc->args);
	current_proc->argv = trim_redirection(&current_proc->argv);
	if (!current_proc->argv)
		return (EXIT_SUCCESS);
	if (!is_builtin(current_proc->argv[0]))
	{
		g_vars.exit_status = get_command_path(&current_proc->argv[0]);
		if (g_vars.exit_status != 0)
		{
			handle_error(g_vars.exit_status, current_proc->argv[0]);
			return (g_vars.exit_status);
		}
	}
	return (current_proc->status);
}

void	update_proc(t_i_mode_vars *i_vars, t_proc_unit *proc_list)
{
	i_vars->pro_count = proc_len(proc_list);
	i_vars->child_pids = malloc(sizeof(pid_t) * i_vars->pro_count);
	ft_bzero(i_vars->child_pids, sizeof(pid_t) * i_vars->pro_count);
	if (!i_vars->child_pids)
	{
		free_proc_list(&proc_list);
		libc_error();
	}
}

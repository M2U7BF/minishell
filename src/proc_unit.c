/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_unit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:41:44 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 16:20:32 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	debug_put_proc_list(t_proc_unit *proc_unit)
{
	t_proc_unit	*current_proc;
	int			i;

	if (!proc_unit)
	{
		printf("(null)\n");
		return ;
	}
	current_proc = proc_unit;
	i = 0;
	while (current_proc)
	{
		printf("[%d] args:\n", i);
		debug_put_token_list(current_proc->args);
		printf("type:%d\n", current_proc->type);
		current_proc = current_proc->next;
		i++;
	}
}

void	free_proc_list(t_proc_unit *proc_list)
{
	t_proc_unit	*current_proc;
	t_proc_unit	*tmp;

	if (!proc_list)
		return ;
	current_proc = proc_list;
	while (current_proc)
	{
		tmp = current_proc;
		current_proc = current_proc->next;
		free_token_list(tmp->args);
		ft_free((void **)&tmp);
	}
}

t_proc_unit	*new_proc(t_token *args, t_proc_unit_type type, int in_fd,
		int out_fd)
{
	t_proc_unit	*proc_unit;

	if (!args)
		return (NULL);
	proc_unit = malloc(sizeof(t_proc_unit));
	if (!proc_unit)
		return (NULL);
	proc_unit->next = NULL;
	proc_unit->args = args;
	proc_unit->type = type;
	proc_unit->read_fd = in_fd;
	proc_unit->write_fd = out_fd;
	return (proc_unit);
}

void	append_proc_unit(t_proc_unit **proc_list, t_proc_unit *proc_unit)
{
	t_proc_unit	*current_proc;

	if (!proc_list || !(*proc_list) || !proc_unit)
		return ;
	current_proc = (*proc_list);
	while (current_proc)
	{
		if (!current_proc->next)
		{
			current_proc->next = proc_unit;
			break ;
		}
		current_proc = current_proc->next;
	}
}

t_proc_unit	*get_prev_proc(t_proc_unit **proc_list, t_proc_unit *proc)
{
	t_proc_unit	*current_token;
	t_proc_unit	*prev;

	if (*proc_list == proc)
		return (NULL);
	current_token = *proc_list;
	prev = NULL;
	while (current_token)
	{
		if (current_token == proc)
			return (prev);
		prev = current_token;
		current_token = current_token->next;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:29:28 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/07 11:26:09 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	debug_put_token_list(t_token *token_list);
// int	debug_put_token_list_compare(t_token *t, t_token *t_e);
// void	debug_put_proc_list(t_proc_unit *proc_unit);
// void	debug_put_lst(t_list *lst);

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
		printf("[%2d] str:[%s], type:%d\n", i, current_token->str,
			current_token->type);
		current_token = current_token->next;
		i++;
	}
}

int	debug_put_token_list_compare(t_token *t, t_token *t_e)
{
	int		i;
	bool	is_not_all_equal;
	bool	is_different;
	t_token	*current_t;
	t_token	*current_t_e;

	if (t == NULL)
	{
		printf("(null)\n");
		return (-1);
	}
	i = 0;
	is_not_all_equal = false;
	current_t = t;
	current_t_e = t_e;
	while (current_t)
	{
		if (!current_t_e)
		{
			printf("current_t_e is null\n");
			break ;
		}
		is_different = ft_strncmp(current_t->str, current_t_e->str,
				ft_strlen(current_t_e->str) + 1) != 0
			|| current_t->type != current_t_e->type;
		if (is_different)
		{
			is_not_all_equal = true;
			printf("[%2d] [❌] result: str=[%s], type=[%d], expected: str=[%s],\
				type=[%d]\n", i, current_t->str, current_t->type,
				current_t_e->str, current_t_e->type);
		}
		else
			printf("[%2d] [⭕] result: str=[%s], type=[%d], expected: str=[%s],\
				type=[%d]\n", i, current_t->str, current_t->type,
				current_t_e->str, current_t_e->type);
		current_t = current_t->next;
		current_t_e = current_t_e->next;
		i++;
	}
	if (!is_not_all_equal)
		printf("すべて同じ 💎\n");
	else
		printf("すべて同じではない 🔥\n");
	return (is_not_all_equal);
}

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

void	debug_put_lst(t_list *lst)
{
	t_list	*current_lst;
	int		i;

	if (!lst)
		dprintf(STDERR_FILENO, "(null)\n");
	current_lst = lst;
	i = 0;
	while (current_lst)
	{
		dprintf(STDERR_FILENO, "[%d] \"%d, %d\"\n", i,
			*(int *)current_lst->content, *(int *)current_lst->content + 1);
		current_lst = current_lst->next;
		i++;
	}
}

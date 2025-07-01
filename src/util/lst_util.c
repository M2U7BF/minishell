/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:50:45 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 15:53:25 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**lst_to_str_arr(t_list *lst)
{
	int		i;
	char	**new;
	t_list	*current_lst;

	if (!lst)
		return (NULL);
	i = 0;
	new = malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!new)
		return (NULL);
	new[ft_lstsize(lst)] = NULL;
	current_lst = lst;
	while (current_lst)
	{
		new[i] = ft_strdup((char *)current_lst->content);
		current_lst = current_lst->next;
		i++;
	}
	return (new);
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

void	del_content(void *content)
{
	ft_free((void **)&content);
}

t_list	*get_prev_lst(t_list **list, t_list *elem)
{
	t_list	*current;
	t_list	*prev;

	if (*list == elem)
		return (NULL);
	current = *list;
	prev = NULL;
	while (current)
	{
		if (current == elem)
			return (prev);
		prev = current;
		current = current->next;
	}
	return (NULL);
}

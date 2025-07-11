/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:50:45 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/07 14:01:18 by atashiro         ###   ########.fr       */
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

void	del_content(void *content)
{
	free(content);
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

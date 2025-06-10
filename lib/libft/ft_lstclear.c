/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 07:44:13 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/10 14:05:16 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Deletes and frees the given node and all its
// successors, using the function ’del’ and free(3).
// Finally, set the pointer to the list to NULL.
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current_lst;
	t_list	*next;

	if (!lst || !*lst || !del)
		return ;
	current_lst = *lst;
	while (current_lst)
	{
		next = current_lst->next;
		ft_lstdelone(current_lst, del);
		current_lst = next;
	}
	*lst = NULL;
}

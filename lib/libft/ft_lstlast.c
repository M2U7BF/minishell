/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 07:44:23 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/10 13:55:14 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current_lst;

	if (!lst)
		return (NULL);
	current_lst = lst;
	while (current_lst->next)
		current_lst = current_lst->next;
	return (current_lst);
}

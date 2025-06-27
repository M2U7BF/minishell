/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_elem.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:12:42 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 11:25:14 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**remove_elem(char **arr, char **remove_list)
{
	int		i;
	t_list	*new_lst;
	char	**new;

	new_lst = NULL;
	i = -1;
	while (arr[++i])
	{
		if (!is_include(arr[i], remove_list))
			ft_lstadd_back(&new_lst, ft_lstnew((void *)ft_strdup(arr[i])));
	}
	new = lst_to_str_arr(new_lst);
	ft_lstclear(&new_lst, del_content);
	return (new);
}

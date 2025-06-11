/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitarr_leave_separator.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:03:12 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/11 15:25:44 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// char **arr: 文字列の配列
// char separator: 分割文字
// separatorにマッチして、配列の文字列を内部的に分割する。
char	**ft_splitarr_leave_separator(char **arr, char separator)
{
	int		i;
	int		j;
	char	**tmp;
	t_list	*new_lst;
	char	**new;

	new_lst = NULL;
	i = -1;
	while (arr[++i])
	{
		if (ft_strchr(arr[i], separator) != NULL)
		{
			tmp = ft_split_leave_separator(arr[i], separator);
			j = 0;
			while (j < arrlen(tmp))
			{
				ft_lstadd_back(&new_lst, ft_lstnew((void *)ft_strdup(tmp[j])));
				j++;
			}
			free_str_array(tmp);
		}
		else
			ft_lstadd_back(&new_lst, ft_lstnew((void *)ft_strdup(arr[i])));
	}
	new = lst_to_str_arr(new_lst);
	ft_lstclear(&new_lst, del_content);
	return (new);
}

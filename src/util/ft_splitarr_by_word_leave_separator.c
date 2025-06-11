/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitarr_by_word_leave_separator.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:39:24 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/11 15:25:38 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_splitarr_by_word_leave_separator(char **arr, char *separator)
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
		// printf("[%d], separator:[%s], arr[i]:[%s]\n", i, separator, arr[i]);
		if (ft_strnstr(arr[i], separator, ft_strlen(arr[i])) != NULL)
		{
			tmp = ft_split_by_word_leave_separator(arr[i], separator);
			// printf("tmp:\n");
			// put_strarr(tmp);
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
	// printf("new_lst:\n");
	// debug_put_lst(new_lst);
	new = lst_to_str_arr(new_lst);
	ft_lstclear(&new_lst, del_content);
	// ft_free(new_lst);
	return (new);
}

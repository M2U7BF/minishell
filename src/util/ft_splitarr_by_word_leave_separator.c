/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitarr_by_word_leave_separator.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:39:24 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/07 13:38:14 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	split(char *s, char *separator, t_list **new_lst)
{
	int		j;
	char	**tmp;

	if (ft_strnstr(s, separator, ft_strlen(s)) != NULL)
	{
		tmp = ft_split_by_word_keep_sep(s, separator);
		j = 0;
		while (j < arrlen(tmp))
			ft_lstadd_back(new_lst, ft_lstnew((void *)ft_strdup(tmp[j++])));
		free_str_array(&tmp);
	}
	else
		ft_lstadd_back(new_lst, ft_lstnew((void *)ft_strdup(s)));
}

char	**ft_splitarr_by_word_leave_separator(char **arr, char *separator)
{
	int		i;
	char	**new;
	t_list	*new_lst;

	if (!arr || !*arr)
		return (NULL);
	i = -1;
	new_lst = NULL;
	while (arr[++i])
		split(arr[i], separator, &new_lst);
	free_str_array(&arr);
	new = lst_to_str_arr(new_lst);
	ft_lstclear(&new_lst, del_content);
	return (new);
}

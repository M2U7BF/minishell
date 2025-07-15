/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitarr_by_words_keep_sep.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:03:25 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 12:09:00 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_include_separator(char *str, char **words)
{
	int	str_len;
	int	k;

	str_len = ft_strlen(str);
	k = -1;
	while (words[++k])
	{
		if (ft_strnstr(str, words[k], str_len))
			return (true);
	}
	return (false);
}

static char	**inner_process(char **arr, char **separators)
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
		if (is_include_separator(arr[i], separators))
		{
			tmp = ft_split_by_words_keep_sep(arr[i], separators);
			j = 0;
			while (j < arrlen(tmp))
				ft_lstadd_back(&new_lst,
					ft_lstnew((void *)ft_strdup(tmp[j++])));
			free_str_array(&tmp);
		}
		else
			ft_lstadd_back(&new_lst, ft_lstnew((void *)ft_strdup(arr[i])));
	}
	new = lst_to_str_arr(new_lst);
	ft_lstclear(&new_lst, del_content);
	return (new);
}

// char **arr: Array of strings
// char **separators: Array of delimiter strings
// Updates the array by splitting based on multiple delimiter strings.
char	**ft_splitarr_by_words_keep_sep(char **arr,
		char **separators)
{
	char	**old;
	char	**new;

	if (!arr || !*arr || !separators)
		return (NULL);
	old = inner_process(arr, separators);
	free_str_array(&arr);
	new = inner_process(old, separators);
	while (arrlen(old) != arrlen(new))
	{
		free_str_array(&old);
		old = new;
		new = inner_process(old, separators);
	}
	free_str_array(&new);
	return (old);
}

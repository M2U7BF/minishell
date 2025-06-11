/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_splitarr_by_word_leave_separator.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:39:24 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/11 13:25:47 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	del_content(void *content)
{
	free(content);
}

static int	is_include_separator(char *str, char **words)
{
	int	str_len;
	int	k;

	str_len = ft_strlen(str);
	k = -1;
	while (words[++k])
	{
		if (ft_strnstr(str, words[k], str_len))
			return (1);
	}
	return (0);
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
		// printf("[%d], separator:[%s], arr[i]:[%s]\n", i, separator, arr[i]);
		if (is_include_separator(arr[i], separators))
		{
			tmp = ft_multi_split_by_word_leave_separator(arr[i], separators);
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

// char **arr: 文字列の配列
// char **separators: 分割文字の配列
// 複数の文字列にマッチして、配列を更新する。
char	**ft_multi_splitarr_by_word_leave_separator(char **arr,
		char **separators)
{
	char	**old;
	char	**new;

	old = inner_process(arr, separators);
	new = inner_process(old, separators);
	while (arrlen(old) != arrlen(new))
	{
		free_str_array(old);
		old = new;
		new = inner_process(old, separators);
	}
	free_str_array(new);
	return (old);
}

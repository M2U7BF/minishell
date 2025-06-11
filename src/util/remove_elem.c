/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_elem.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:12:42 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/11 14:28:13 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	del_content(void *content)
{
	free(content);
}

static int is_remove_target(char *s, char **remove_list)
{
  int i;
  int str_len = ft_strlen(s);

  i = -1;
  while (remove_list[++i])
  {
    if (ft_strncmp(s, remove_list[i], str_len + 1) == 0)
      return (1);
  }
  return (0);
}

char **remove_elem(char **arr, char **remove_list)
{
  int		i;
	t_list	*new_lst;
	char	**new;

	new_lst = NULL;
	i = -1;
	while (arr[++i])
	{
		if (!is_remove_target(arr[i], remove_list))
			ft_lstadd_back(&new_lst, ft_lstnew((void *)ft_strdup(arr[i])));
	}
	new = lst_to_str_arr(new_lst);
	ft_lstclear(&new_lst, del_content);
	return (new);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:59:14 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/30 11:09:37 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free(void **p)
{
	free(*p);
	*p = NULL;
}

void	free_str_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		ft_free((void **)&array[i]);
		i++;
	}
	ft_free((void **)&array);
}

char	*ft_strjoin_all(char **words)
{
	int		i;
	char	*result;
	char	*old;

	if (!words)
		return (NULL);
	if (!words[0])
		return (NULL);
	i = -1;
	result = ft_strdup("");
	while (words[++i])
	{
		old = result;
		result = ft_strjoin(result, words[i]);
		ft_free((void **)&old);
	}
	return (result);
}

int	arrlen(char **arr)
{
	int	i;

	if (arr == NULL)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	count_chr(char *s, char c)
{
	int	i;
	int	count;

	if (!s)
		return (0);
	count = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] == c)
			count++;
	}
	return (count);
}

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

void	debug_put_lst(t_list *lst)
{
	t_list	*current_lst;
	int		i;

	if (!lst)
		dprintf(STDERR_FILENO, "(null)\n");
	current_lst = lst;
	i = 0;
	while (current_lst)
	{
		dprintf(STDERR_FILENO, "[%d] \"%d, %d\"\n", i,
			*(int *)current_lst->content, *(int *)current_lst->content + 1);
		current_lst = current_lst->next;
		i++;
	}
}

void	del_content(void *content)
{
	ft_free((void **)&content);
}

bool	is_include(char *s, char **words)
{
	int	i;
	int	str_len;

	str_len = ft_strlen(s);
	i = -1;
	while (words[++i])
	{
		if (ft_strncmp(s, words[i], str_len + 1) == 0)
			return (true);
	}
	return (false);
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

bool	is_strequal(char *s1, char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s1)) == 0);
}

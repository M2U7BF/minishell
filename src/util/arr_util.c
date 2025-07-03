/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:51:44 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 15:52:34 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:59:14 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/04 10:42:17 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free(void *p)
{
	free(p);
	p = NULL;
}

void	free_str_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i])
	{
		ft_free(array[i]);
		i++;
	}
	ft_free(array);
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
		free(old);
	}
	return (result);
}

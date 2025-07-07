/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:20:25 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 11:25:58 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_env_array(char **env_array)
{
	int	count;

	count = 0;
	while (env_array && env_array[count])
		count++;
	return (count);
}

static void	swap_env_items(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort_env_array(char **env_array)
{
	int	i;
	int	j;
	int	count;

	count = count_env_array(env_array);
	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strncmp(env_array[i], env_array[j], -1) > 0)
				swap_env_items(&env_array[i], &env_array[j]);
			j++;
		}
		i++;
	}
}

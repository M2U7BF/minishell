/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:24:14 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/11 16:24:24 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_var_name(char *s, int *end)
{
	int		i;
	char	*name;

	i = 0;
	name = malloc(sizeof(char) * (ft_strlen(s) + 1));
	while (ft_isalnum(s[i]) || s[i] == '_')
		i++;
	*end = i;
	ft_strlcpy(name, s, i + 1);
	return (name);
}

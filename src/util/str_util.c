/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:51:18 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 17:12:44 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

bool	is_str_equal(char *s1, char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s1) + 1) == 0);
}

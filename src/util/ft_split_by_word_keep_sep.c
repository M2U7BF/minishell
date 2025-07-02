/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_word_keep_sep.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:40:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 13:49:32 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_split_by_word_keep_sep(char *str, char *word)
{
	char	**res;
	int		i;
	int		j;
	char	*tmp;
	int		k;

	i = -1;
	j = 0;
	k = 0;
	if (!str || !word || !*word)
		return (NULL);
	res = malloc(sizeof(char *) * (ft_strlen(str) + 1));
	if (!res)
		return (NULL);
	while (++i <= (int)ft_strlen(str))
	{
		if (i - k > 0 && (is_str_equal(word, str + i, 0) || !str[i]))
		{
			tmp = malloc(sizeof(char) * (i - k + 1));
			ft_strlcpy(tmp, str + k, i - k + 1);
			res[j++] = tmp;
		}
		if (is_str_equal(word, str + i, 0))
		{
			res[j++] = ft_strdup(word);
			k = i + ft_strlen(word);
		}
	}
	res[j] = NULL;
	return (res);
}

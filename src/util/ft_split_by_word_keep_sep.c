/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_word_keep_sep.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:40:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 13:39:48 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_split_by_word_keep_sep(char *str, char *word)
{
	char	**res;
	int		i;
	int		j;
	int		len;
	char	*tmp;
	int		k;

	i = -1;
	j = 0;
	k = 0;
	len = ft_strlen(str);
	if (!str || !word || !*word)
		return (NULL);
	res = malloc(sizeof(char *) * (len + 1));
	if (!res)
		return (NULL);
	while (++i <= len)
	{
		if (ft_strncmp(str + i, word, ft_strlen(word)) == 0)
		{
			if (i != 0 && i - k > 0)
			{
				tmp = malloc(sizeof(char) * (i - k + 1));
				ft_strlcpy(tmp, str + k, i - k + 1);
				res[j++] = tmp;
			}
			res[j++] = ft_strdup(word);
			k = i + ft_strlen(word);
		}
		else if (str[i] == '\0' && i - k > 0)
		{
			tmp = malloc(sizeof(char) * (i - k + 1));
			ft_strlcpy(tmp, str + k, i - k + 1);
			res[j++] = tmp;
		}
	}
	res[j] = NULL;
	return (res);
}

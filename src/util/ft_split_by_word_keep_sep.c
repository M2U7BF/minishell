/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_word_keep_sep.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:40:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 17:40:49 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_all(char **result, int result_len)
{
	int	i;

	i = 0;
	while (result && i <= result_len && result[i])
	{
		ft_free((void **)&result[i]);
		i++;
	}
	ft_free((void **)&result);
}

char	**ft_split_by_word_keep_sep(char *str, char *word)
{
	char	**res;
	int		i;
	int		j;
	int		len;
	char	*word_position;
	char	*tmp;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	if (!str || !word || !*word)
		return (NULL);
	res = malloc(sizeof(char *) * (len + 1));
	if (!res)
		return (NULL);
	while (i < len)
	{
		word_position = ft_strnstr(str + i, word, len - i);
		if (word_position)
		{
			if (word_position != str + i)
			{
				tmp = malloc(word_position - (str + i) + 1);
				if (!tmp)
					return (free_all(res, j - 1), NULL);
				ft_strlcpy(tmp, str + i, word_position - (str + i) + 1);
				res[j++] = tmp;
			}
			res[j++] = ft_strdup(word);
			i = word_position - str + ft_strlen(word);
		}
		else
		{
			res[j++] = ft_strdup(str + i);
			break ;
		}
	}
	res[j] = NULL;
	return (res);
}

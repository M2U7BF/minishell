/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_word_keep_sep.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:40:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 12:56:36 by kkamei           ###   ########.fr       */
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

static char	*get_x(char *word_position, char *str, int i)
{
	char	*tmp;

	tmp = malloc(word_position - str + i + 1);
	if (!tmp)
		return (NULL);
	ft_strlcpy(tmp, str + i, word_position - (str + i) + 1);
	return (tmp);
}

char	**ft_split_by_word_keep_sep(char *str, char *word)
{
	char	**res;
	int		i;
	int		j;
	int		len;
	char	*word_position;

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
				res[j] = get_x(word_position, str, i);
				if (!res[j++])
					return (free_all(res, j - 1), NULL);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_words_keep_sep.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:48:43 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/30 09:45:13 by kkamei           ###   ########.fr       */
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

char	**ft_split_by_words_keep_sep(char *str, char **words)
{
	int		i;
	int		j;
	int		k;
	int		l;
	int		str_len;
	char	*word_position;
	char	**result;
	char	*result_str;
	bool	is_end;

	if (!str || !words || !words[0] || words[0][0] == '\0')
		return (NULL);
	str_len = ft_strlen(str);
	result = malloc(sizeof(char *) * (str_len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < str_len)
	{
		k = -1;
		word_position = NULL;
		while (!word_position && words[++k])
		{
			if (ft_strncmp(str + i, words[k], ft_strlen(words[k])) == 0)
				word_position = str + i;
		}
		if (word_position)
		{
			if (str + i != word_position)
			{
				result_str = malloc(word_position - str + 1);
				if (!result_str)
					return (free_all(result, j - 1), NULL);
				ft_strlcpy(result_str, str + i, word_position - (str + i) + 1);
				result[j++] = result_str;
				i += word_position - (str + i);
			}
			result[j++] = ft_strdup(words[k]);
			i += ft_strlen(words[k]);
		}
		else
		{
			l = 0;
			is_end = false;
			while (!is_end && i + ++l < str_len)
			{
				k = -1;
				while (words[++k] && !is_end)
					is_end = ft_strncmp(str + i + l, words[k],
							ft_strlen(words[k])) == 0;
			}
			result_str = malloc(l + 1);
			if (!result_str)
				return (free_all(result, j - 1), NULL);
			ft_strlcpy(result_str, str + i, l + 1);
			result[j++] = result_str;
			i += l;
		}
	}
	result[j] = NULL;
	return (result);
}

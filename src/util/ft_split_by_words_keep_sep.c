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

static int	get_not_word_part(char *str, char **words, int i)
{
	int		k;
	int		l;
	bool	is_end;

	k = 0;
	l = 0;
	is_end = false;
	while (!is_end && i + ++l < (int)ft_strlen(str))
	{
		k = -1;
		while (!is_end && words[++k])
			is_end = ft_strncmp(str + i + l, words[k],
					ft_strlen(words[k])) == 0;
	}
	return (l);
}

static char	*split(char *str, char **words, int *i)
{
	int		k;
	int		l;
	char	*result_str;
	char	*word_position;

	k = -1;
	word_position = NULL;
	while (words[++k])
	{
		if (ft_strncmp(str + *i, words[k], ft_strlen(words[k])) == 0)
		{
			word_position = str + *i;
			*i += ft_strlen(words[k]);
			return (ft_strdup(words[k]));
		}
	}
	l = get_not_word_part(str, words, *i);
	result_str = malloc(l + 1);
	if (!result_str)
		return (NULL);
	ft_strlcpy(result_str, str + *i, l + 1);
	*i += l;
	return (result_str);
}

char	**ft_split_by_words_keep_sep(char *str, char **words)
{
	int		i;
	int		j;
	int		str_len;
	char	**result;

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
		result[j] = split(str, words, &i);
		if (!result[j])
			return (free_all(result, j - 1), NULL);
		j++;
	}
	result[j] = NULL;
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_split_by_word_leave_separator.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:48:43 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/26 08:52:40 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_multi_split_by_word_leave_separator(char *str, char **words)
{
	int		i;
	int		j;
	int		k;
	int		l;
	int		str_len;
	int		count;
	char	*word_position;
	char	**result;
	char	*result_str;
	bool	is_end;

	is_end = false;
	if (!str || !words || !words[0] || words[0][0] == '\0')
		return (NULL);
	str_len = ft_strlen(str);
	i = 0;
	count = 0;
	while (i < str_len)
	{
		k = -1;
		word_position = NULL;
		while (words[++k])
		{
			if (ft_strncmp(str + i, words[k], ft_strlen(words[k])) == 0)
				word_position = str + i;
			if (word_position)
				break ;
		}
		if (word_position)
		{
			if (str + i != word_position)
			{
				i += word_position - str - i;
				count++;
			}
			i += ft_strlen(words[k]);
			count++;
		}
		else
		{
			l = 1;
			is_end = false;
			while (i + l < str_len)
			{
				k = -1;
				while (words[++k] && !is_end)
				{
					if (ft_strncmp(str + i + l, words[k],
							ft_strlen(words[k])) == 0)
						is_end = true;
				}
				if (is_end)
					break ;
				l++;
			}
			i += l;
			count++;
		}
	}
	result = malloc(sizeof(char *) * (count + 1));
  if (!result)
    return (NULL);
	i = 0;
	j = 0;
	while (i < str_len)
	{
		k = -1;
		word_position = NULL;
		while (words[++k])
		{
			if (ft_strncmp(str + i, words[k], ft_strlen(words[k])) == 0)
				word_position = str + i;
			if (word_position)
				break ;
		}
		if (word_position)
		{
			if (str + i != word_position)
			{
				result_str = malloc(word_position - str + 1);
        if (!result_str)
          return (NULL);
				ft_strlcpy(result_str, str + i, word_position - (str + i) + 1);
				result[j] = result_str;
				j++;
				i += word_position - (str + i);
			}
			result[j] = ft_strdup(words[k]);
			i += ft_strlen(words[k]);
			j++;
		}
		else
		{
			l = 1;
			is_end = false;
			while (i + l < str_len)
			{
				k = -1;
				while (words[++k] && !is_end)
				{
					if (ft_strncmp(str + i + l, words[k],
							ft_strlen(words[k])) == 0)
						is_end = true;
				}
				if (is_end)
					break ;
				l++;
			}
			result_str = malloc(l + 1);
      if (!result_str)
        return (NULL);
			ft_strlcpy(result_str, str + i, l + 1);
			result[j] = result_str;
			j++;
			i += l;
		}
	}
	result[j] = NULL;
	return (result);
}

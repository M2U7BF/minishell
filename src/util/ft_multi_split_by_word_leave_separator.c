/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_split_by_word_leave_separator.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:48:43 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/11 10:05:01 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// wordsの先頭から一致するか試し、一致したらすぐ返す。
// static char	*get_word_position(char **words, char *str, int i)
// {
// 	int		j;
// 	int		str_len;
// 	char	*word_position;

// 	j = -1;
// 	str_len = ft_strlen(str);
// 	while (words[++j])
// 	{
// 		word_position = ft_strnstr(str + i, words[j], str_len);
// 		if (word_position)
// 			return (word_position);
// 	}
// 	return (NULL);
// }

char	**ft_multi_split_by_word_leave_separator(char *str, char **words)
{
	int		i;
	int		j;
	int		k;
	int		str_len;
	int		count;
	char	*word_position;
	char	**result;
	char	*result_str;

	if (!str || !words || !words[0] || words[0][0] == '\0')
		return (NULL);
	str_len = ft_strlen(str);
	i = 0;
	count = 0;
	while (i < str_len)
	{
		k = -1;
		while (words[++k])
		{
			word_position = ft_strnstr(str + i, words[k], str_len);
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
			i += str_len - i;
			count++;
		}
	}
	result = malloc(sizeof(char *) * (count + 1));
	i = 0;
	j = 0;
	while (i < str_len)
	{
		k = -1;
		while (words[++k])
		{
			word_position = ft_strnstr(str + i, words[k], str_len);
			if (word_position)
				break ;
		}
		if (word_position)
		{
			if (str + i != word_position)
			{
				result_str = malloc(word_position - str + 1);
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
			result_str = malloc(str_len - i + 2);
			ft_strlcpy(result_str, str + i, str_len - i + 2);
			result[j] = result_str;
			j++;
			i += str_len - i + 1;
		}
	}
	result[j] = NULL;
	return (result);
}

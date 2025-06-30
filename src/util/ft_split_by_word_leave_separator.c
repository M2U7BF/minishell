/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_word_leave_separator.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:48:43 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/30 09:45:18 by kkamei           ###   ########.fr       */
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

char	**ft_split_by_word_leave_separator(char *str, char *word)
{
	int		i;
	int		j;
	int		str_len;
	int		count;
	char	*word_position;
	char	**result;
	char	*result_str;

	if (!str || !word || word[0] == '\0')
		return (NULL);
	str_len = ft_strlen(str);
	i = 0;
	count = 0;
	while (i < str_len)
	{
		word_position = ft_strnstr(str + i, word, str_len);
		if (word_position)
		{
			if (str + i != word_position)
			{
				i += word_position - str - i;
				count++;
			}
			i += ft_strlen(word);
			count++;
		}
		else
		{
			i += str_len - i;
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
		word_position = ft_strnstr(str + i, word, str_len);
		if (word_position)
		{
			if (str + i != word_position)
			{
				result_str = malloc(word_position - str + 1);
				if (!result_str)
				{
					free_all(result, j - 1);
					return (NULL);
				}
				ft_strlcpy(result_str, str + i, word_position - (str + i) + 1);
				result[j] = result_str;
				j++;
				i += word_position - (str + i);
			}
			result[j] = ft_strdup(word);
			i += ft_strlen(word);
			j++;
		}
		else
		{
			result_str = malloc(str_len - i + 2);
			if (!result_str)
			{
				free_all(result, j - 1);
				return (NULL);
			}
			ft_strlcpy(result_str, str + i, str_len - i + 2);
			result[j] = result_str;
			j++;
			i += str_len - i + 1;
		}
	}
	result[j] = NULL;
	return (result);
}

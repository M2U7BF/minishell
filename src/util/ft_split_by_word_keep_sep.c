/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_word_keep_sep.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:40:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/02 14:07:00 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	split(int i, int *k, char *str, char *word, char ***res_tmp)
{
	char	*tmp;

	if (i - *k > 0 && (is_str_equal(word, str + i, 0) || !str[i]))
	{
		tmp = malloc(sizeof(char) * (i - *k + 1));
		ft_strlcpy(tmp, str + *k, i - *k + 1);
		**res_tmp = tmp;
		(*res_tmp)++;
	}
	if (is_str_equal(word, str + i, 0))
	{
		**res_tmp = ft_strdup(word);
		(*res_tmp)++;
		*k = i + ft_strlen(word);
	}
}

char	**ft_split_by_word_keep_sep(char *str, char *word)
{
	char	**result;
	char	**res_tmp;
	int		i;
	int		k;

	i = -1;
	k = 0;
	if (!str || !word || !*word)
		return (NULL);
	result = malloc(sizeof(char *) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	res_tmp = result;
	while (++i <= (int)ft_strlen(str))
		split(i, &k, str, word, &res_tmp);
	*res_tmp = NULL;
	return (result);
}

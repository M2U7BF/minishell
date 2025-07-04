/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_word_keep_sep.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:40:33 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/04 13:29:40 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	split(int i, int *k, t_split_vars vars, char ***res_tmp)
{
	char	*tmp;

	if (i - *k > 0 && (is_str_equal(vars.str + i, vars.word, false)
			|| !vars.str[i]))
	{
		tmp = malloc(sizeof(char) * (i - *k + 1));
		ft_strlcpy(tmp, vars.str + *k, i - *k + 1);
		**res_tmp = tmp;
		(*res_tmp)++;
	}
	if (is_str_equal(vars.str + i, vars.word, false))
	{
		**res_tmp = ft_strdup(vars.word);
		(*res_tmp)++;
		*k = i + ft_strlen(vars.word);
	}
}

char	**ft_split_by_word_keep_sep(char *str, char *word)
{
	t_split_vars	vars;
	char			**result;
	char			**res_tmp;
	int				i;
	int				k;

	i = -1;
	k = 0;
	if (!str || !word || !*word)
		return (NULL);
	vars.str = str;
	vars.word = word;
	result = malloc(sizeof(char *) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	res_tmp = result;
	while (++i <= (int)ft_strlen(str))
		split(i, &k, vars, &res_tmp);
	*res_tmp = NULL;
	return (result);
}

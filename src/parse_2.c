/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:24:14 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/28 10:33:00 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_var_name(char *s, int *end)
{
	int		i;
	char	*name;

	i = 0;
	name = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (ft_isalpha(s[0]) || s[0] == '_')
	{
		while (ft_isalnum(s[i]) || s[i] == '_')
			i++;
	}
	*end = i;
	ft_strlcpy(name, s, i + 1);
	return (name);
}

void	expand_question_mark(char **s)
{
	char	**tmp;

	tmp = ft_split_by_word_keep_sep(*s, "$?");
	ft_free((void **)&tmp[0]);
	tmp[0] = ft_itoa(access_exit_status(false, 0));
	ft_free((void **)s);
	*s = ft_strjoin_all(tmp);
	free_str_array(&tmp);
}

void	expand_variable(char **s)
{
	char	*var;
	char	*name;
	int		end;
	char	*new;

	end = 0;
	name = get_var_name(*s + 1, &end);
	var = get_env_value(access_env_list(false, NULL), name);
	new = NULL;
	if (!name[0])
		new = ft_strdup(*s);
	else if (var)
	{
		new = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(*s) - end + 1));
		ft_strlcpy(new, var, ft_strlen(var) + 1);
		ft_strlcpy(new + ft_strlen(var), *s + end + 1, ft_strlen(*s) - end + 1);
	}
	else
	{
		new = malloc(sizeof(char) * (ft_strlen(*s) - end + 1));
		ft_strlcpy(new, *s + end + 1, ft_strlen(*s) - end + 1);
	}
	ft_free((void **)s);
	*s = new;
	ft_free((void **)&name);
}

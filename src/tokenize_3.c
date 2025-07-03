/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:29:34 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 10:52:43 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_blank(char *s)
{
	int			i;
	static char	*chars[] = {" ", "\t", NULL};
	static int	len = sizeof(chars) / sizeof(chars[0]);

	if (!s)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

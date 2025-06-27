/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:29:34 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 15:18:37 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_blank(char *s)
{
	int			i;
	static char	*chars[] = BLANK_LIST;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:04:30 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 11:25:23 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_export(const char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	s++;
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

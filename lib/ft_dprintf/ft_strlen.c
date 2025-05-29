/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:39:53 by kkamei            #+#    #+#             */
/*   Updated: 2024/11/10 10:25:35 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

// #include <stdio.h>
// #include <string.h>
// int	main()
// {
// 	char *str = "test test test";
// 	printf("[%s] a:%ld, b:%lld\n", str, strlen(str), ft_strlen(str));
// }

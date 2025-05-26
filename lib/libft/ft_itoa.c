/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 08:42:28 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/29 08:40:52 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*convert_to_str(long *digits, int len, int is_minus);

char	*ft_itoa(int n)
{
	int		i;
	int		is_minus;
	long	digits[10];
	long	num;

	i = 0;
	is_minus = 0;
	num = (long)n;
	if (num < 0)
	{
		is_minus = 1;
		num *= -1;
	}
	while (num >= 10)
	{
		digits[i] = num % 10;
		num /= 10;
		i++;
	}
	digits[i] = num;
	return (convert_to_str(digits, i, is_minus));
}

static char	*convert_to_str(long *digits, int len, int is_minus)
{
	int		i;
	char	*str;

	i = 0;
	if (is_minus)
	{
		i++;
		str = malloc(sizeof(char) * (len + 3));
	}
	else
		str = malloc(sizeof(char) * (len + 2));
	if (!str)
		return (NULL);
	if (is_minus)
		str[0] = '-';
	while (len >= 0)
	{
		str[i] = (int)digits[len] + '0';
		i++;
		len--;
	}
	str[i] = '\0';
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 11:15:17 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/26 22:42:55 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	ft_strtol(const char *nptr);
static int	ft_isspace(int c);
static long	convert_to_long(int i, char *digits, int sign);

int	ft_atoi(const char *nptr)
{
	return ((int)ft_strtol(nptr));
}

static int	ft_isspace(int c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static long	ft_strtol(const char *nptr)
{
	int		i;
	int		j;
	int		sign;
	char	num[20];

	i = 0;
	j = 0;
	sign = 1;
	while (ft_isspace((int)nptr[i]))
		i++;
	if ((char)nptr[i] == '+' || (char)nptr[i] == '-')
	{
		if (((char)nptr[i] == '-'))
			sign = -1;
		i++;
	}
	while ((char)nptr[i] == '0')
		i++;
	while (ft_isdigit((int)nptr[i]) && j < 20)
	{
		num[j] = (char)nptr[i];
		i++;
		j++;
	}
	return (convert_to_long(j, num, sign));
}

static long	convert_to_long(int num_count, char *digits, int sign)
{
	unsigned long long	n;
	int					j;

	n = 0;
	j = 0;
	while (j < num_count)
	{
		n = n * 10 + (unsigned long long)(digits[j] - '0');
		j++;
	}
	if (n > (unsigned long long)LONG_MAX)
	{
		if (sign == -1)
			return (LONG_MIN);
		return (LONG_MAX);
	}
	return (((long)n) * sign);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintnbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:00:05 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:27:20 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom.h"

int	ft_dprintnbr(int n, int fd)
{
	int	bytes;

	bytes = 0;
	if (n < 0)
	{
		if (n == -2147483648)
			return (ft_dprintstr("-2147483648", fd));
		bytes += ft_dprintchr('-', fd);
		n *= -1;
	}
	if (n >= 10)
		bytes += ft_dprintnbr(n / 10, fd);
	bytes += ft_dprintchr(n % 10 + '0', fd);
	return (bytes);
}

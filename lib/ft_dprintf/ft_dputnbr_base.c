/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dputnbr_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 11:04:59 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:27:30 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom.h"

int	ft_dputnbr_base(unsigned long long nbr, char *base, int fd)
{
	int		digits[20];
	int		num_count;
	size_t	base_len;
	int		bytes;
	int		write_result;

	num_count = 0;
	base_len = ft_strlen(base);
	bytes = 0;
	while (nbr >= base_len)
	{
		digits[num_count++] = nbr % base_len;
		nbr /= base_len;
	}
	digits[num_count++] = nbr;
	while (num_count > 0)
	{
		write_result = write(fd, &base[digits[num_count - 1]], 1);
		if (write_result == -1)
			return (-1);
		bytes += write_result;
		num_count--;
	}
	return (bytes);
}

// #include <stdio.h>
// int main()
// {
// 	printf("---------- TEST ----------\n");
// 	ft_putnbr_base(9999, "AB");
// 	printf("\n");
// 	ft_putnbr_base(9999, "0123456789");
// 	printf("\n");
// 	ft_putnbr_base(9999, "0123456789ABCDEF");
// 	printf("\n");
// 	ft_putnbr_base(9999, "poneyvif");
// 	printf("\n");
// 	ft_putnbr_base(-9999, "poneyvif");
// 	printf("\n");
// 	printf("---------- ERROR TEST ----------\n");
// 	ft_putnbr_base(-9999, "");
// 	printf("\n");
// 	ft_putnbr_base(-9999, "0");
// 	printf("\n");
// 	ft_putnbr_base(-9999, "poneyviff");
// 	printf("\n");
// 	ft_putnbr_base(-9999, "pponeyvif");
// 	printf("\n");
// 	ft_putnbr_base(-9999, "1+A");
// 	printf("\n");
// 	ft_putnbr_base(-9999, "1-V");
// 	printf("\n");
// }

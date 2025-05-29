/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:34:32 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:27:05 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom.h"

int	ft_dprintp(void *p, int fd)
{
	unsigned long	num;
	int				bytes;

	bytes = 0;
	num = (unsigned long)p;
	if (num == 0)
		return (write(fd, "(nil)", 5));
	bytes += write(fd, "0x", 2);
	bytes += ft_dputnbr_lhex(num, fd);
	return (bytes);
}

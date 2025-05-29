/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintunbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:47:29 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:21:07 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom.h"

int	ft_dprintunbr(unsigned int n, int fd)
{
	int	bytes;

	bytes = 0;
	if (n >= 10)
		bytes += ft_dprintunbr(n / 10, fd);
	bytes += ft_dprintchr(n % 10 + '0', fd);
	return (bytes);
}

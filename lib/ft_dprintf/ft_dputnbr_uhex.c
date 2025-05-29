/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dputnbr_uhex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 11:04:59 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:21:19 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom.h"

int	ft_dputnbr_uhex(unsigned long long nbr, int fd)
{
	char	*base;

	base = "0123456789ABCDEF";
	return (ft_dputnbr_base(nbr, base, fd));
}

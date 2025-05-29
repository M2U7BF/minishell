/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dputnbr_lhex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 11:04:59 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:21:14 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom.h"

int	ft_dputnbr_lhex(unsigned long long nbr, int fd)
{
	char	*base;

	base = "0123456789abcdef";
	return (ft_dputnbr_base(nbr, base, fd));
}

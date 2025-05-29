/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:33:17 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:47:58 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static int	ft_dprint_fmt(char c, int fd, va_list ap)
{
	if (c == 'c')
		return (ft_dprintchr(va_arg(ap, int), fd));
	else if (c == 's')
		return (ft_dprintstr(va_arg(ap, const char *), fd));
	else if (c == 'p')
		return (ft_dprintp(va_arg(ap, void *), fd));
	else if (c == 'd' || c == 'i')
		return (ft_dprintnbr(va_arg(ap, int), fd));
	else if (c == 'u')
		return (ft_dprintunbr(va_arg(ap, unsigned int), fd));
	else if (c == 'x')
		return (ft_dputnbr_lhex(va_arg(ap, unsigned int), fd));
	else if (c == 'X')
		return (ft_dputnbr_uhex(va_arg(ap, unsigned int), fd));
	else if (c == '%')
		return (ft_dprintchr('%', fd));
	return (0);
}

int	ft_dprintf(int fd, const char *fmt, ...)
{
	va_list	ap;
	int		size;
	int		tmp_size;
	char	*t_fmt;

	size = 0;
	t_fmt = (char *)fmt;
	va_start(ap, fmt);
	while (*t_fmt)
	{
		if (*t_fmt == '%')
		{
			t_fmt++;
			tmp_size = ft_dprint_fmt(*t_fmt, fd, ap);
		}
		else
			tmp_size = write(fd, t_fmt, 1);
		if (tmp_size == -1)
			break ;
		size += tmp_size;
		t_fmt++;
	}
	va_end(ap);
	return (size);
}

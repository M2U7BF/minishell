/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:20:56 by kkamei            #+#    #+#             */
/*   Updated: 2025/05/02 11:18:57 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUSTOM_H
# define CUSTOM_H

# include <stdlib.h>
# include <unistd.h>

int		ft_dprintstr(const char *s, int fd);
int		ft_dputnbr_base(unsigned long long nbr, char *base, int fd);
int		ft_dprintchr(unsigned char c, int fd);
int		ft_dprintnbr(int n, int fd);
int		ft_dprintp(void *p, int fd);
int		ft_dprintunbr(unsigned int n, int fd);
int		ft_dputnbr_lhex(unsigned long long nbr, int fd);
int		ft_dputnbr_uhex(unsigned long long nbr, int fd);
size_t	ft_strlen(const char *str);

#endif
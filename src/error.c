/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:50:05 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 17:56:00 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	put_error_exit(char *s, int status)
{
	ft_putstr_fd(s, STDERR_FILENO);
	exit(status);
}

void	libc_error(char *s)
{
  dprintf(STDERR_FILENO, "libc_error: %s:", s);
	put_error_exit(strerror(errno), EXIT_FAILURE);
}

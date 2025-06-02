/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:55:12 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/02 16:58:13 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ctrl_backslash(int signum)
{
	// TODO 内容実装
	printf("ctrl_backslash, signum:%d\n", signum);
}

void	ctrl_c(int signum)
{
	// TODO 内容実装
	printf("ctrl_c, signum:%d\n", signum);
	exit(0);
}

void	handle_signal(void)
{
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, ctrl_backslash);
}

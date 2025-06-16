/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:55:12 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/16 14:58:15 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile	sig_atomic_t g_recieve_signal = 0;
// void	ctrl_backslash(int signum)
// {
// 	// TODO 内容実装
// 	printf("ctrl_backslash, signum:%d\n", signum);
// }

// void	ctrl_c(int signum)
// {
// 	// TODO 内容実装
// 	(void)signum;
// 	exit(EOWNERDEAD);
// }

/*受け取ったシグナルの番号をグルーバル変数に入れる*/
void	signal_hander(int signum)
{
	g_recieve_signal = signum;
}

void	handle_signal(void)
{
	signal(SIGINT, signal_hander);
	signal(SIGQUIT, SIG_IGN);
}

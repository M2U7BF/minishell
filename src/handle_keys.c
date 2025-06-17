/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:55:12 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/17 18:51:37 by atashiro         ###   ########.fr       */
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
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signal(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = signal_hander;
	if(sigaction(SIGINT, &sa_int, NULL) < 0)
		put_error_exit("sigaction", EXIT_FAILURE);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	if(sigaction(SIGQUIT, &sa_quit, NULL) < 0)
		put_error_exit("sigaction", EXIT_FAILURE);
}

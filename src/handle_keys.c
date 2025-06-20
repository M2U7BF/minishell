/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:55:12 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/20 07:42:05 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// volatile sig_atomic_t	g_recieve_signal = 0;

/*受け取ったシグナルの番号をグルーバル変数に入れる*/
void	signal_handler(int signum)
{
	g_recieve_signal = signum;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signal_handler(int signum, void (*handler)(int))
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if (sigaction(signum, &sa, NULL) < 0)
		put_error_exit("sigaction", EXIT_FAILURE);
}

void	handle_signal(void)
{
	set_signal_handler(SIGINT, signal_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}


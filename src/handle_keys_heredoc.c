/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:41:20 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/24 09:29:05 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	signal_handler(int signum)
{
	g_recieve_signal = signum;
	if (g_recieve_signal == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_done = 1;
	}
}

static void	set_signal_handler(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if (sigaction(signum, &sa, NULL) < 0)
		put_error_exit("sigaction", EXIT_FAILURE);
}

void	handle_signal_heredoc(void)
{
	set_signal_handler(SIGINT, signal_handler);
}

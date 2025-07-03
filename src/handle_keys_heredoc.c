/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 08:41:20 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 17:17:35 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	signal_handler(int signum)
{
	g_vars.signal = signum;
	g_vars.exit_status = 128 + signum;
	if (g_vars.signal == SIGINT)
	{
		if (write(STDERR_FILENO, "\n", 1) == -1)
			libc_error();
		rl_replace_line("", 0);
		rl_done = 1;
	}
}

static void	set_signal_handler(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) == -1)
		libc_error();
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if (sigaction(signum, &sa, NULL) < 0)
		put_error_exit("sigaction", EXIT_FAILURE);
}

void	set_heredoc_signal_handlers(void)
{
	set_signal_handler(SIGINT, signal_handler);
	g_vars.signal = 0;
}

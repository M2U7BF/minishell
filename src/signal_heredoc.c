/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:58:03 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/23 16:41:18 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_state(void)
{
	if (g_signum == 0)
		return (0);
	else if (g_signum == SIGINT)
	{
		g_signum = 0;
		return (0);
	}
	return (0);
}

static void	signal_handler(int val)
{
	g_signum = val;
	access_exit_status(true, 128 + g_signum);
	if (g_signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
}

static void	set_signal_handler(int g_signum, void (*handler)(int))
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) == -1)
		libc_error();
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	if (sigaction(g_signum, &sa, NULL) < 0)
		put_error_exit("sigaction", EXIT_FAILURE);
}

void	set_heredoc_signal_handlers(void)
{
	if (isatty(STDIN_FILENO))
		rl_event_hook = check_state;
	set_signal_handler(SIGINT, signal_handler);
	g_signum = 0;
}

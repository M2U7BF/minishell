/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:58:03 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 16:59:56 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_state(void)
{
	if (signum == 0)
		return (0);
	else if (signum == SIGINT)
	{
		signum = 0;
		return (0);
	}
	return (0);
}

static void	signal_handler(int val)
{
	signum = val;
	access_exit_status(true, 128 + signum);
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
}

static void	set_signal_handler(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	if (sigemptyset(&sa.sa_mask) == -1)
		libc_error();
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	if (sigaction(signum, &sa, NULL) < 0)
		put_error_exit("sigaction", EXIT_FAILURE);
}

void	set_heredoc_signal_handlers(void)
{
	if (isatty(STDIN_FILENO))
		rl_event_hook = check_state;
	set_signal_handler(SIGINT, signal_handler);
	signum = 0;
}

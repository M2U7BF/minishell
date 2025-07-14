/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:58:03 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/14 16:04:07 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_state(void)
{
	if (g_vars.signal == 0)
		return (0);
	else if (g_vars.signal == SIGINT)
	{
		g_vars.signal = 0;
		g_vars.interrupted = true;
		return (0);
	}
	return (0);
}

static void	signal_handler(int signum)
{
	g_vars.signal = signum;
	g_vars.exit_status = 128 + signum;
	if (g_vars.signal == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
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
	rl_event_hook = check_state;
	set_signal_handler(SIGINT, signal_handler);
	g_vars.signal = 0;
}

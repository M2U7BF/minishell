/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:57:38 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/15 15:17:21 by kkamei           ###   ########.fr       */
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
		return (0);
	}
	return (0);
}

static void	sigint_handler_prompt(int signum)
{
	g_vars.signal = signum;
	g_vars.exit_status = 128 + signum;
	if (g_vars.signal == SIGINT)
	{
		g_vars.interrupted = 1;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
}

static void	sigint_handler_exec(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	set_signal_handlers(bool is_exec)
{
	struct sigaction	sa;

	if (isatty(STDIN_FILENO))
		rl_event_hook = check_state;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (is_exec)
		sa.sa_handler = sigint_handler_exec;
	else
		sa.sa_handler = sigint_handler_prompt;
	sigaction(SIGINT, &sa, NULL);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		libc_error();
	g_vars.signal = 0;
}

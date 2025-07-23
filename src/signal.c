/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:57:38 by kkamei            #+#    #+#             */
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

static void	sigint_handler_prompt(int val)
{
	g_signum = val;
	access_exit_status(true, 128 + g_signum);
	if (g_signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
}

static void	sigint_handler_exec(int g_signum)
{
	(void)g_signum;
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
	g_signum = 0;
}

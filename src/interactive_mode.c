/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/24 15:42:52 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_recieve_signal = 0;

void	wait_child_processes(int *child_pids, int pro_count, int *exit_status)
{
	int	status;
	int	i;

	i = 0;
	while (i < pro_count)
	{
		waitpid(child_pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			*exit_status = 128 + SIGQUIT;
		}
		else if (WTERMSIG(status) == SIGINT)
			*exit_status = 128 + SIGINT;
	}
	else
		put_error_exit("waitpid", EXIT_FAILURE);
}

int	exec_interactive(t_exec_vars *e_vars)
{
	t_i_mode_vars	*i_vars;
	int				status;

	status = 0;
	init_i_vars(&e_vars->i_vars);
	i_vars = &e_vars->i_vars;
	rl_outstream = stderr;
	g_recieve_signal = 0;
	handle_signal();
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		g_recieve_signal = 0;
		i_vars->input_line = readline(i_vars->prompt);
		// Ctrl+Cのあとに、Ctrl+Dを打った場合
		if (!i_vars->input_line)
		{
			if (g_recieve_signal == SIGINT)
				status = 130;
			exit(status);
		}
		if (i_vars->input_line[0] != '\0')
			add_history(i_vars->input_line);
		else
		{
			ft_free(i_vars->input_line);
			continue ;
		}
		i_vars->input_line = ft_strtrim_front(i_vars->input_line,
				DEFAULT_BLANK);
		if (is_quotation_error(i_vars->input_line) != 0)
			exit(EXIT_FAILURE);
		// 単語分割
		i_vars->token_list = tokenize(i_vars->input_line);
		// tokenize後の構文エラーを検知する
		// printf("tokenize後\n");
		// debug_put_token_list(i_vars->token_list);
		if (!i_vars->token_list)
		{
			ft_free(i_vars->input_line);
			continue ;
		}
		if (is_syntax_error(i_vars->token_list))
		{
			ft_dprintf(STDERR_FILENO, "syntax_error\n");
			exit(EXIT_SYNTAX_ERROR);
		}
		// パース
		parse(i_vars);
		// printf("quote_removal前\n");
		// debug_put_token_list(i_vars->token_list);
		quote_removal(i_vars->token_list);
		// printf("exec前\n");
		// debug_put_token_list(i_vars->token_list);
		// コマンド実行
		exec(i_vars);
		wait_child_processes(i_vars->child_pids, i_vars->pro_count, &status);
		handle_signal();
		ft_free(i_vars->input_line);
		free_token_list(i_vars->token_list);
		ft_free(i_vars->child_pids);
		i_vars->child_pids = NULL;
	}
	return (0);
}

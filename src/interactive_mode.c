/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/05 17:40:06 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	else
		put_error_exit("waitpid", EXIT_FAILURE);
}

int	exec_interactive(t_exec_vars *e_vars)
{
	t_i_mode_vars	*i_vars;
	static int		proc_size = 1;
	int				status;

	handle_signal();
	init_i_vars(&e_vars->i_vars);
	i_vars = &e_vars->i_vars;
	rl_outstream = stderr;
	while (1)
	{
		i_vars->input_line = readline(i_vars->prompt);
		if (!i_vars->input_line)
			exit(EXIT_SUCCESS);
		if (i_vars->input_line[0] != '\0')
			add_history(i_vars->input_line);
		// 単語分割
		i_vars->token_list = tokenize(i_vars->input_line);
		// TODO 単語分割の結果から、全体のプロセスの数を計算しmalloc
    i_vars->pro_count = proc_size;
		i_vars->child_pids = malloc(sizeof(pid_t) * i_vars->pro_count);
		// パース
		parse(i_vars);
		// debug_put_token_list(i_vars->token_list);
		// コマンド実行
		exec(i_vars);
		wait_child_processes(i_vars->child_pids, i_vars->pro_count, &status);
		ft_free(i_vars->input_line);
	}
}

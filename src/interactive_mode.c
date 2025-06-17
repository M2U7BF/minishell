/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/16 16:25:28 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern volatile	sig_atomic_t g_recieve_signal;

void	wait_child_processes(int *child_pids, int pro_count, int *exit_status)
{
	int	status;
	int	i;

	i = 0;
	while (i < pro_count)
	{
		waitpid(child_pids[i], &status, 0);
    // reset_redirect();
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
		g_recieve_signal = 0;

		i_vars->input_line = readline(i_vars->prompt);
		/*Ctrl+C*/
		if(g_recieve_signal == SIGINT)
		{
			status = 130;
			ft_putstr_fd("\n", STDOUT_FILENO);
			rl_on_new_line();
			rl_replace_line("", 0);
			// rl_redisplay();
			ft_free(i_vars->input_line);
			continue;
		}
		/*Ctrl+D*/
		if (!i_vars->input_line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			exit(EXIT_SUCCESS);
		}
		if (i_vars->input_line[0] != '\0')
			add_history(i_vars->input_line);
		if (check_quotation(i_vars->input_line) != 0)
			exit(EXIT_FAILURE);
		// 単語分割
		i_vars->token_list = tokenize(i_vars->input_line);
    // tokenize後の構文エラーを検知する
    // printf("tokenize後\n");
    // debug_put_token_list(i_vars->token_list);
		if (check_syntax_error(i_vars->token_list) != 0)
			exit(EXIT_SYNTAX_ERROR);
		// TODO 単語分割の結果から、全体のプロセスの数を計算しmalloc
		i_vars->pro_count = proc_size;
		i_vars->child_pids = malloc(sizeof(pid_t) * i_vars->pro_count);
		// パース
		parse(i_vars);
		quote_removal(i_vars->token_list);
    // printf("exec前\n");
		// debug_put_token_list(i_vars->token_list);
		/*（要検討）実行中のコマンドのために親プロセスのを無視（要検討）*/
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		// コマンド実行
		exec(i_vars);
		wait_child_processes(i_vars->child_pids, i_vars->pro_count, &status);
		handle_signal();

		// Ctrl+\ (SIGQUIT) で子プロセスが終了したかチェック
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		{
			ft_putendl_fd("Quit: 3", STDERR_FILENO);
			/*TODO: ここでシェルの終了ステータスを131に更新*/
		}


    // TODO heredocのファイルを削除する
    	unlink(TMPFILE_NAME);
		ft_free(i_vars->input_line);
	}
}

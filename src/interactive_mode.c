/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/20 08:26:02 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// extern volatile sig_atomic_t	g_recieve_signal;

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
	int				status;

	status = 0;
	init_i_vars(&e_vars->i_vars);
	i_vars = &e_vars->i_vars;
	rl_outstream = stderr;
	g_recieve_signal = 0;
	handle_signal();
	while (1)
	{
		g_recieve_signal = 0;
		i_vars->input_line = readline(i_vars->prompt);
		/*Ctrl+D*/
		if (!i_vars->input_line)
		{
			{
				// readlineがNULLを返したのが、Ctrl+Cによるものかチェック
				if (g_recieve_signal == SIGINT)
				{
					status = 130; // BashはCtrl+Cで終了ステータスを130にする
					continue;     // ループを継続して新しいプロンプトを表示
				}
				// ft_putendl_fd("Ctrl+Dが押されました", STDOUT_FILENO);
                // printf("プロセスID %d がexit()を呼び出します。\n", getpid());
                // fflush(stdout); // printfのバッファを強制的に出力
				ft_putendl_fd("exit!", STDOUT_FILENO);
				exit(status); // 最後のコマンドの終了ステータスで終了するのが望ましい
			}
			// exit(EXIT_SUCCESS); // シェルを終了
		}
		if (i_vars->input_line[0] != '\0')
			add_history(i_vars->input_line);
		else
		{
			ft_free(i_vars->input_line); // 空行のメモリを解放
			continue ;                    // ループの先頭に戻り、新しいプロンプトを表示
		}
		if (check_quotation(i_vars->input_line) != 0)
			exit(EXIT_FAILURE);
		// 単語分割
		i_vars->token_list = tokenize(i_vars->input_line);
		// tokenize後の構文エラーを検知する
		// printf("tokenize後\n");
		// debug_put_token_list(i_vars->token_list);
		if (check_syntax_error(i_vars->token_list) != 0)
		{
			ft_dprintf(STDERR_FILENO, "syntax_error\n");
			exit(EXIT_SYNTAX_ERROR);
		}
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
		ft_free(i_vars->input_line);
		free_token_list(i_vars->token_list);
		ft_free(i_vars->child_pids);
		i_vars->child_pids = NULL;
	}
	return (0);
}

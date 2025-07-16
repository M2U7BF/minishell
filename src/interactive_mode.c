/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:39:01 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/16 16:54:34 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_runtime_data	g_vars = {};

static void	process_ctrl_d(void)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	free_env_list(&g_vars.env_list);
	rl_clear_history();
	exit(g_vars.exit_status);
}

void	process_heredoc(t_proc_unit *proc_list)
{
	t_proc_unit	*cur_p;
	t_token		*cur_t;
	int			i;
	int			j;
	int			fd;
	int			status;
	static char	*pathname_arr[] = {TMP_PATH, "", "_", "", NULL};
	char		*tmp_file_path;

	i = 0;
	cur_p = proc_list;
	while (cur_p)
	{
		j = 0;
		cur_t = cur_p->args;
		cur_p->heredoc_tmp_paths = malloc(sizeof(char *)
				* (count_heredoc(cur_p->args) + 1));
		while (cur_t && cur_t->next)
		{
			fd = -1;
			if (is_redir_pair(cur_t) && is_s_eq(cur_t->str, "<<", true))
			{
				pathname_arr[1] = ft_itoa(i);
				pathname_arr[3] = ft_itoa(j);
				//   1. 一時ファイル作成（>fileのイメージで）
				//     1. tmp_<プロセス番号>_<ヒアドキュメント番号> とかで
				tmp_file_path = ft_strjoin_all(pathname_arr);
        ft_free((void **)&pathname_arr[1]);
        ft_free((void **)&pathname_arr[3]);
				open_outfile(tmp_file_path, &fd);
				//   2. 標準入力を一時ファイルへ向ける
				redirect(&fd, STDOUT_FILENO, &cur_p->redirect_fds);
				status = here_doc(ft_strdup(cur_t->next->str), STDOUT_FILENO);
				//   3. 入力完了
				//   4. プロセスの構造体にファイルパスを保持
				//     1. 配列とかで保持するようにするか...
				cur_p->heredoc_tmp_paths[j] = tmp_file_path;
				//   5. 標準入力をリセット
				reset_redirection(&cur_p->redirect_fds);
				cur_t = cur_t->next;
        j++;
			}
			cur_t = cur_t->next;
		}
    cur_p->heredoc_tmp_paths[j] = NULL;
		cur_p = cur_p->next;
		i++;
	}
}

int	exec_interactive(t_i_mode_vars *i_vars)
{
	t_proc_unit	*proc_list;

	rl_outstream = stderr;
	while (1)
	{
		g_vars.interrupted = 0;
		i_vars->prompt = get_prompt();
		if (i_vars->input)
			ft_free((void **)&i_vars->input);
		set_signal_handlers(false);
		i_vars->input = readline(i_vars->prompt);
		ft_free((void **)&i_vars->prompt);
		if (g_vars.interrupted)
			continue ;
		if (!i_vars->input)
			process_ctrl_d();
		else if (i_vars->input[0] == '\0')
			continue ;
		add_history(i_vars->input);
		ft_strtrim_front(&i_vars->input, DEFAULT_BLANK);
		i_vars->token_list = tokenize(i_vars->input);
		if (!i_vars->token_list)
			continue ;
		if (parse(i_vars) == -1)
			continue ;
		proc_list = process_division(i_vars);
		process_heredoc(proc_list);
		exec(i_vars, proc_list, EXIT_SUCCESS);
		destroy_i_vars(i_vars);
	}
	return (0);
}

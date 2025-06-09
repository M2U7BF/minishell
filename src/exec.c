/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/09 12:55:41 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_readable_file(char *pathname)
{
	int		fd;
	char	buf;
	ssize_t	bytes;

	fd = open(pathname, O_RDONLY);
	if (fd == -1)
		return (0);
	bytes = read(fd, &buf, 1);
	if (bytes == -1)
		return (close(fd), 0);
	return (close(fd), 1);
}

static int	search_command_path(char **cmd_name, char **path_env)
{
	int		i;
	char	*path;
	char	*tmp;

	i = 0;
	while (path_env && path_env[i])
	{
		tmp = ft_strjoin(path_env[i], "/");
		path = ft_strjoin(tmp, *cmd_name);
		free(tmp);
		if (access(path, F_OK) != 0)
		{
			free(path);
			i++;
			continue ;
		}
		free(*cmd_name);
		*cmd_name = path;
		if (access(*cmd_name, R_OK) != 0)
			return (EACCES);
		if (!is_readable_file(*cmd_name))
			return (EISDIR);
		return (0);
	}
	return (EXIT_CMD_NOT_FOUND);
}

int	get_command_path(char **cmd_name)
{
	char	**path_env;
	int		status;

	path_env = ft_split(getenv("PATH"), ':');
	status = 0;
	if ((*cmd_name)[0] == '\0')
		status = EXIT_CMD_NOT_FOUND;
	else if ((*cmd_name)[0] == '/')
	{
		if (access(*cmd_name, F_OK) != 0)
			status = ENOENT;
		else if (access(*cmd_name, R_OK) != 0)
			status = EACCES;
		else if (!is_readable_file(*cmd_name))
			status = EISDIR;
	}
	else if (ft_strncmp(*cmd_name, ".", 2) == 0)
		status = EISDIR;
	else if (ft_strncmp(*cmd_name, "..", 3) == 0)
		status = EISDIR;
	else
		status = search_command_path(cmd_name, path_env);
	free_str_array(path_env);
	return (status);
}

// TODO ここでprocess数をカウントするようにしてもいいかも？
t_proc_unit	*process_division(t_token *token_list)
{
	t_token		*current_token;
	t_proc_unit	*proc_list;

	if (!token_list)
		return (NULL);
	current_token = token_list;
	proc_list = NULL;
	while (current_token)
	{
		if (current_token->type == WORD)
		{
			if (!proc_list)
				proc_list = create_proc_unit(token_dup(current_token),
						SIMPLE_CMD);
			else
				append_token(&proc_list->args, token_dup(current_token));
		}
		else
		{
			// TODO 複数プロセスへの対応
			printf("");
		}
		current_token = current_token->next;
	}
	return (proc_list);
}

int	is_redirection(char *s)
{
	int			i;
	static char	*chars[] = {">", "<", ">>", "<<"};
	static int	len = sizeof(chars) / sizeof(chars[0]);

	if (!s)
		return (0);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
			return (1);
	}
	return (0);
}

int	open_additionalfile(char *filename, int *fd)
{
	if (filename[0] == '\0')
		return (ENOENT);
	if (access(filename, F_OK) == 0)
	{
		if (access(filename, R_OK) == -1)
			return (EACCES);
		if (!is_readable_file(filename))
			return (EISDIR);
	}
  *fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (*fd == -1)
		return (EXIT_FAILURE);
	return (0);
}

int	open_outfile(char *filename, int *fd)
{
	if (filename[0] == '\0')
		return (ENOENT);
	if (access(filename, F_OK) == 0)
	{
		if (access(filename, R_OK) == -1)
			return (EACCES);
		if (!is_readable_file(filename))
			return (EISDIR);
	}
	*fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (*fd == -1)
		return (EXIT_FAILURE);
	return (0);
}

int	open_infile(char *filename, int *fd)
{
	if (access(filename, F_OK) == -1)
		return (ENOENT);
	if (access(filename, R_OK) == -1)
		return (EACCES);
	*fd = open(filename, O_RDONLY);
	if (*fd == -1)
		return (EXIT_FAILURE);
	return (0);
}

// TODO: 下記に対応させる
// <redirect_out> = '>' <word>
// <redirect_in> = '<' <word>
// <redirect_append> = '>>' <word>
// <redirect_heredoc> = '<<' <word>
void	open_files(char **argv)
{
	int	i;
	int	out_fd;
	int	in_fd;

	i = -1;
	out_fd = 0;
  in_fd = 0;
	while (argv[++i] && argv[i + 1])
	{
		if (is_redirection(argv[i]), is_word(argv[i + 1]))
		{
			if (ft_strncmp(argv[i + 1], ">", 2) != NULL)
			{
				// TODO: fdは先に計算する？
				open_outfile(argv[i + 1], &out_fd);
			}
			else if (ft_strncmp(argv[i + 1], "<", 2) != NULL)
			{
				// TODO: 
        open_infile(argv[i + 1], &in_fd);
			}
			else if (ft_strncmp(argv[i + 1], ">>", 3) != NULL)
			{
				// 追加出力
        open_additionalfile(argv[i + 1], &out_fd);
			}
			else if (ft_strncmp(argv[i + 1], "<<", 3) != NULL)
			{
				//　ヒアドキュメント
			}
		}
	}
}

int	exec(t_i_mode_vars *i_vars)
{
	int			i;
	t_proc_unit	*proc_list;
	t_proc_unit	*current_proc;
	char		**argv;

	// TODO 制御演算子が見つかるごとに、みたいな処理でいいかも
	// TODO プロセスごとにforkして実行
	proc_list = process_division(i_vars->token_list);
	i = -1;
	current_proc = proc_list;
	while (++i < i_vars->pro_count)
	{
		i_vars->child_pids[i] = fork();
		if (i_vars->child_pids[i] == 0)
		{
			argv = tokens_to_arr(current_proc->args);
			open_redirect_files(argv); // 必要なfileをopenしたり、heredocの場合はpipeを作成したり
			redirect();                // dupを用いて、fdのredirectを行う
			// コマンドパス取得
			get_command_path(&argv[0]);
			// 実行
			execve(argv[0], argv, __environ);
			perror("execve");
			return (EXIT_FAILURE);
		}
		current_proc = current_proc->next;
	}
	return (0);
}

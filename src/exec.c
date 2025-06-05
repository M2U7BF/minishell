/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/05 17:18:38 by kkamei           ###   ########.fr       */
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

int	exec(t_i_mode_vars *i_vars)
{
	int		i;
	t_token	*current_token;
	char	***exec_args;
	int		word_count;

	// TODO 制御演算子が見つかるごとに、みたいな処理でいいかも
	// TODO プロセスごとにforkして実行
	i = 0;
	word_count = 0;
	exec_args = malloc(sizeof(char **) * (i_vars->pro_count + 1));
	if (!exec_args)
		return (EXIT_FAILURE);
	exec_args[i_vars->pro_count] = NULL;
	current_token = i_vars->token_list;
	// cmd + arg + arg ...の配列を作成する
	while (current_token)
	{
		if (current_token->type == WORD)
			word_count++;
		else
		{
			exec_args[i] = malloc(sizeof(char *) * (word_count + 1));
			exec_args[i][word_count] = NULL;
			word_count = 0;
			i++;
		}
		current_token = current_token->next;
	}
	exec_args[i] = malloc(sizeof(char *) * (word_count + 1));
	exec_args[i][word_count] = NULL;
	current_token = i_vars->token_list;
	i = 0;
	word_count = 0;
	while (current_token)
	{
		if (current_token->type == WORD)
		{
			exec_args[i][word_count] = current_token->str;
			word_count++;
		}
		else
		{
			word_count = 0;
			i++;
		}
		current_token = current_token->next;
	}
	i = -1;
	while (++i < i_vars->pro_count)
	{
		i_vars->child_pids[i] = fork();
		if (i_vars->child_pids[i] == 0)
		{
			// コマンドパス取得
			get_command_path(&exec_args[i][0]);
			// 実行
			execve(exec_args[i][0], exec_args[i], i_vars->envp);
			perror("execve");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

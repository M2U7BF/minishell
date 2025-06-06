/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/06 17:49:49 by kkamei           ###   ########.fr       */
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
				proc_list = create_proc_unit(token_dup(current_token), SIMPLE_CMD);
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

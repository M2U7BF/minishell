/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 19:55:26 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		ft_free(tmp);
		if (access(path, F_OK) != 0)
		{
			ft_free(path);
			i++;
			continue ;
		}
		ft_free(*cmd_name);
		*cmd_name = path;
		if (access(*cmd_name, X_OK) != 0)
			return (errno);
		return (0);
	}
	if (access(*cmd_name, F_OK) == 0 && access(*cmd_name, R_OK) != 0)
		return (errno);
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
	else if (ft_strchr((*cmd_name), '/'))
	{
		if (access(*cmd_name, X_OK) != 0)
			status = errno;
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
t_proc_unit	*process_division(t_token *token_list, int *pro_count)
{
	t_token		*current_token;
	t_proc_unit	*result;
	t_proc_unit	*current_proc;
	int			i;

	if (!token_list)
		return (NULL);
	current_token = token_list;
	result = NULL;
	current_proc = NULL;
	i = 0;
	while (current_token)
	{
		if (current_token->type == PIPE)
		{
			current_token = current_token->next;
			current_proc->next = create_proc_unit(token_dup(current_token),
					PIPE_LINE, STDIN_FILENO, STDOUT_FILENO);
			current_proc = current_proc->next;
			i++;
		}
		else if (!current_proc)
		{
			current_proc = create_proc_unit(token_dup(current_token),
					SIMPLE_CMD, STDIN_FILENO, STDOUT_FILENO);
			result = current_proc;
			i++;
		}
		else
			append_token(&current_proc->args, token_dup(current_token));
		current_token = current_token->next;
	}
	*pro_count = i;
	return (result);
}

bool	is_invalid_fd(int fd)
{
	struct stat	sb;

	if (fstat(fd, &sb) == -1 && errno == EBADF)
		return (true);
	return (false);
}

int	stashfd(int fd)
{
	int	stashfd;

	if (fd == -1)
		return (-1);
	stashfd = 10;
	while (1)
	{
		if (is_invalid_fd(stashfd))
			break ;
		stashfd++;
	}
	if (dup2(fd, stashfd) == -1)
		libc_error();
	if (close(fd) == -1)
		libc_error();
	return (stashfd);
}

// 必要なfileをopenし、リダイレクトを行う。
int	open_and_redirect_files(t_proc_unit *current_proc, t_list **redirect_fds, t_list *env_list)
{
	int		*fd;
	int		target_fd;
	int		stashed_target_fd;
	t_token	*current;
	int		*content;
	int		status;

	status = 0;
	current = current_proc->args;
	while (current && current->next)
	{
		if (current->type == REDIRECTION && (current->next->type == WORD
				|| current->next->type == DELIMITER))
		{
			if (!current->next->str)
			{
				ft_dprintf(STDERR_FILENO, "minishell: ambiguous redirect\n");
				return (EXIT_FAILURE);
			}
			fd = malloc(sizeof(int));
			if (!fd)
				return (EXIT_FAILURE);
			content = malloc(sizeof(int) * 2);
			if (!content)
			{
				ft_free(fd);
				return (EXIT_FAILURE);
			}
			if (ft_strncmp(current->str, ">", 2) == 0)
			{
				status = open_outfile(current->next->str, fd);
				if (status != 0)
				{
					ft_free(fd);
					ft_free(content);
					handle_error(status, current->next->str);
					return (status);
				}
				*fd = stashfd(*fd);
				target_fd = STDOUT_FILENO;
			}
			else if (ft_strncmp(current->str, "<", 2) == 0)
			{
				status = open_infile(current->next->str, fd);
				if (status != 0)
				{
					ft_free(fd);
					ft_free(content);
					handle_error(status, current->next->str);
					return (status);
				}
				*fd = stashfd(*fd);
				target_fd = STDIN_FILENO;
			}
			else if (ft_strncmp(current->str, ">>", 3) == 0)
			{
				status = open_additionalfile(current->next->str, fd);
				if (status != 0)
				{
					ft_free(fd);
					ft_free(content);
					handle_error(status, current->next->str);
					return (status);
				}
				*fd = stashfd(*fd);
				target_fd = STDOUT_FILENO;
			}
			else if (ft_strncmp(current->str, "<<", 3) == 0)
			{
				*fd = here_doc(current->next->str, env_list);
				*fd = stashfd(*fd);
				target_fd = STDIN_FILENO;
			}
			stashed_target_fd = stashfd(target_fd);
			if (*fd != target_fd)
			{
				if (dup2(*fd, target_fd) == -1)
					libc_error();
				if (close(*fd) == -1)
					libc_error();
			}
			ft_free(fd);
			content[0] = stashed_target_fd;
			content[1] = target_fd;
			ft_lstadd_back(redirect_fds, ft_lstnew((void *)content));
			current = current->next;
		}
		current = current->next;
	}
	return (status);
}

char	**trim_redirection(char ***argv)
{
	int		i;
	char	**new;
	t_list	*tmp_lst;
	int		len;

	if (!argv || !(*argv))
		return (NULL);
	i = 0;
	tmp_lst = NULL;
	len = arrlen((*argv));
	while (i < len)
	{
		if (is_redirection((*argv)[i]) && (*argv)[i + 1])
		{
			i += 2;
			continue ;
		}
		ft_lstadd_back(&tmp_lst, ft_lstnew((void *)ft_strdup((*argv)[i])));
		i++;
	}
	free_str_array((*argv));
	new = lst_to_str_arr(tmp_lst);
	ft_lstclear(&tmp_lst, del_content);
	return (new);
}

void	reset_redirection(t_list *redirect_fds)
{
	t_list	*current;
	int		*content;

	if (!redirect_fds)
		return ;
	current = redirect_fds;
	while (current->next)
		current = current->next;
	while (1)
	{
		content = (int *)current->content;
		if (dup2(content[0], content[1]) == -1)
			libc_error();
		if (close(content[0]) == -1)
			libc_error();
		if (current == redirect_fds)
			break ;
		current = get_prev_lst(&redirect_fds, current);
	}
	ft_lstclear(&redirect_fds, del_content);
}

t_list	*pipe_redirect(t_proc_unit *proc, t_list *redirect_fds)
{
	int	target_fd;
	int	stashed_target_fd;
	int	*content;

	if (!proc)
		return (redirect_fds);
	target_fd = 0;
	if (proc->read_fd != STDIN_FILENO)
	{
		content = malloc(sizeof(int) * 2);
		if (!content)
		{
			ft_lstclear(&redirect_fds, del_content);
			return (NULL);
		}
		proc->read_fd = stashfd(proc->read_fd);
		target_fd = STDIN_FILENO;
		stashed_target_fd = stashfd(target_fd);
		if (dup2(proc->read_fd, target_fd) == -1)
			libc_error();
		if (close(proc->read_fd) == -1)
			libc_error();
		content[0] = stashed_target_fd;
		content[1] = target_fd;
		ft_lstadd_back(&redirect_fds, ft_lstnew((void *)content));
	}
	if (proc->write_fd != STDOUT_FILENO)
	{
		content = malloc(sizeof(int) * 2);
		if (!content)
		{
			ft_lstclear(&redirect_fds, del_content);
			libc_error();
		}
		proc->write_fd = stashfd(proc->write_fd);
		target_fd = STDOUT_FILENO;
		stashed_target_fd = stashfd(target_fd);
		if (dup2(proc->write_fd, target_fd) == -1)
			libc_error();
		if (close(proc->write_fd) == -1)
			libc_error();
		content[0] = stashed_target_fd;
		content[1] = target_fd;
		ft_lstadd_back(&redirect_fds, ft_lstnew((void *)content));
	}
	return (redirect_fds);
}

void	close_pipe(t_proc_unit *proc)
{
	if (proc->read_fd != STDIN_FILENO)
	{
		if (close(proc->read_fd) == -1)
			libc_error();
	}
	if (proc->write_fd != STDOUT_FILENO)
	{
		if (close(proc->write_fd) == -1)
			libc_error();
	}
}

void	handle_error(int status, char *cmd_path)
{
	if (status == EXIT_CMD_NOT_FOUND)
		ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd_path);
	else if (status == ENOENT)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd_path,
			strerror(status));
		g_runtime_data.exit_status = EXIT_CMD_NOT_FOUND;
	}
	else if (status == EACCES || status == EISDIR)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd_path,
			strerror(status));
		g_runtime_data.exit_status = EXIT_PERMISSION_DENIED;
	}
	else
		perror("minishell");
}

int	exec(t_i_mode_vars *i_vars, t_list *env_list)
{
	int			i;
	t_proc_unit	*proc_list;
	t_proc_unit	*current_proc;
	char		**argv;
	t_list		*redirect_fds;
	int			pipe_fds[2];
	int			status;

	proc_list = process_division(i_vars->token_list, &i_vars->pro_count);
	//------------------------------------------atashiro
	if (!proc_list)
		return (0);
	argv = tokens_to_arr(proc_list->args);
	// パイプがなく、コマンドが "cd" または "exit" の場合
	if (proc_list->type == SIMPLE_CMD && argv != NULL &&
		(ft_strncmp(argv[0], "cd", 3) == 0 ||
		 ft_strncmp(argv[0], "exit", 5) == 0 ||
		 ft_strncmp(argv[0], "export", 7) == 0 ||
		 ft_strncmp(argv[0], "unset", 6) == 0))
	{
		redirect_fds = NULL;
		// redirect_fds = open_and_redirect_files(proc_list, redirect_fds);------mergeしたら型が変わっていて迷子
		char **trimmed_argv = trim_redirection(&argv); // argvはここで消費される
		status = exec_builtin(trimmed_argv, env_list); // 親プロセスで実行
		g_runtime_data.exit_status = status;//追加？-------------------------------要検討
		free_str_array(trimmed_argv);
		reset_redirection(redirect_fds);
		free_proc_list(proc_list);
		// 注意: builtin_exitはプロセスを終了するので、ここに戻らない可能性がある
		return (status);
	}
	free_str_array(argv);
	// ------------------------------------------------------------------
	i_vars->child_pids = malloc(sizeof(pid_t) * i_vars->pro_count);
	if (!i_vars->child_pids)
	{
		free_proc_list(proc_list);
		libc_error();
	}
	// printf("process_divisionの後\n");
	// debug_put_proc_list(proc_list);
	i = -1;
	current_proc = proc_list;
	// dprintf(STDERR_FILENO, "pid:%d\n", getpid());
	while (proc_list && ++i < i_vars->pro_count)
	{
		redirect_fds = NULL;
		if (current_proc->next && current_proc->next->type == PIPE_LINE)
		{
			if (pipe(pipe_fds) == -1)
				libc_error();
			current_proc->write_fd = pipe_fds[1];
			current_proc->next->read_fd = pipe_fds[0];
		}
		redirect_fds = pipe_redirect(current_proc, redirect_fds);
		status = open_and_redirect_files(current_proc, &redirect_fds, env_list);
		i_vars->child_pids[i] = fork();
		if (i_vars->child_pids[i] == 0)
		{
			if (status != 0)
			{
				destroy_i_vars(i_vars);
				free_proc_list(proc_list);
				exit(EXIT_FAILURE);
			}
			if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
				libc_error();
			if (current_proc->next
				&& current_proc->next->read_fd != STDIN_FILENO)
			{
				if (close(current_proc->next->read_fd) == -1)
					libc_error();
			}
			argv = tokens_to_arr(current_proc->args);
			// printf("argv1:\n");
			// put_strarr(argv);
			argv = trim_redirection(&argv);
			// printf("argv2:\n");
			// put_strarr(argv);
			if (!argv)
				exit(EXIT_SUCCESS);
			//atashiro-----------------------
			if (is_builtin(argv[0]))
			{
				status = exec_builtin(argv, env_list);//追加env
				exit(status);
			}
			//-------------
			g_runtime_data.exit_status = get_command_path(&argv[0]);
			if (g_runtime_data.exit_status != 0)
			{
				handle_error(g_runtime_data.exit_status, argv[0]);
				exit(g_runtime_data.exit_status);
			}
			char **envp_array = convert_env_list_to_array(env_list);
			execve(argv[0], argv, envp_array);
			free_str_array(envp_array); // execveが失敗した場合
			perror("execve");
			exit(EXIT_CMD_NOT_FOUND);
		}
		else if (i_vars->child_pids[i] == -1)
			libc_error();
		else
		{
			if (signal(SIGINT, SIG_IGN) == SIG_ERR)
				libc_error();
			reset_redirection(redirect_fds);
		}
		current_proc = current_proc->next;
	}
	free_proc_list(proc_list);
	return (0);
}

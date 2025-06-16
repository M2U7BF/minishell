/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:57:04 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/16 11:43:39 by kkamei           ###   ########.fr       */
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
t_proc_unit	*process_division(t_token *token_list, int *pro_count)
{
	t_token		*current_token;
	t_proc_unit	*result;
	t_proc_unit	*current_proc;
	int			pipe_fds[2];
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
			pipe(pipe_fds);
			current_proc->out_fd = pipe_fds[1];
			current_token = current_token->next;
			current_proc->next = create_proc_unit(token_dup(current_token),
					PIPE_LINE, pipe_fds[0], STDOUT_FILENO);
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

int	stashfd(int fd)
{
	int	stashfd;

	stashfd = fcntl(fd, F_DUPFD, 10);
	if (stashfd < 0)
		perror("fcntl");
	close(fd);
	return (stashfd);
}

// 必要なfileをopenし、リダイレクトを行う。
t_list	*open_and_redirect_files(t_token *argv, t_list *redirect_fds)
{
	int		*fd;
	int		target_fd;
	int		stashed_target_fd;
	t_token	*current;
	int		*content;

	current = argv;
	while (current && current->next)
	{
		if (current->type == REDIRECTION && current->next->type == WORD)
		{
			fd = malloc(sizeof(int));
			content = malloc(sizeof(int) * 2);
			if (ft_strncmp(current->str, ">", 2) == 0)
			{
				open_outfile(current->next->str, fd);
				*fd = stashfd(*fd);
				target_fd = STDOUT_FILENO;
			}
			else if (ft_strncmp(current->str, "<", 2) == 0)
			{
				open_infile(current->next->str, fd);
				*fd = stashfd(*fd);
				target_fd = STDIN_FILENO;
			}
			else if (ft_strncmp(current->str, ">>", 3) == 0)
			{
				open_additionalfile(current->next->str, fd);
				*fd = stashfd(*fd);
				target_fd = STDOUT_FILENO;
			}
			else if (ft_strncmp(current->str, "<<", 3) == 0)
			{
				*fd = here_doc(current->next->str);
				*fd = stashfd(*fd);
				target_fd = STDIN_FILENO;
			}
			stashed_target_fd = stashfd(target_fd);
			if (*fd != target_fd)
			{
				dup2(*fd, target_fd);
				close(*fd);
			}
			content[0] = stashed_target_fd;
			content[1] = target_fd;
			ft_lstadd_back(&redirect_fds, ft_lstnew((void *)content));
			current = current->next;
		}
		current = current->next;
	}
	return (redirect_fds);
}

char	**trim_redirection(char ***argv)
{
	char	**new;
	int		i;

	if (!argv || !(*argv))
		return (NULL);
	i = 0;
	while ((*argv)[i] && !is_redirection((*argv)[i]))
		i++;
	if (i == 0)
		return ((*argv));
	new = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while ((*argv)[i] && !is_redirection((*argv)[i]))
	{
		new[i] = ft_strdup((*argv)[i]);
		i++;
	}
	new[i] = NULL;
	free_str_array((*argv));
	return (new);
}

static void	reset_redirection(t_list *redirect_fds)
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
		dup2(content[0], content[1]);
		if (current == redirect_fds)
			break ;
		current = get_prev_lst(&redirect_fds, current);
	}
	ft_lstclear(&current, del_content);
}

t_list	*pipe_redirect(t_proc_unit *proc, t_list *redirect_fds)
{
	int	target_fd;
	int	stashed_target_fd;
	int	*content;

	if (!proc)
		return (redirect_fds);
	target_fd = 0;
	if (proc->in_fd != STDIN_FILENO)
	{
		content = malloc(sizeof(int) * 2);
		proc->in_fd = stashfd(proc->in_fd);
		target_fd = STDIN_FILENO;
		stashed_target_fd = stashfd(target_fd);
		dup2(proc->in_fd, target_fd);
		close(proc->in_fd);
		content[0] = stashed_target_fd;
		content[1] = target_fd;
		ft_lstadd_back(&redirect_fds, ft_lstnew((void *)content));
	}
	if (proc->out_fd != STDOUT_FILENO)
	{
		content = malloc(sizeof(int) * 2);
		proc->out_fd = stashfd(proc->out_fd);
		target_fd = STDOUT_FILENO;
		stashed_target_fd = stashfd(target_fd);
		dup2(proc->out_fd, target_fd);
		close(proc->out_fd);
		content[0] = stashed_target_fd;
		content[1] = target_fd;
		ft_lstadd_back(&redirect_fds, ft_lstnew((void *)content));
	}
	return (redirect_fds);
}

void	close_pipe(t_proc_unit *proc)
{
	if (proc->in_fd != STDIN_FILENO)
		close(proc->in_fd);
	if (proc->out_fd != STDOUT_FILENO)
		close(proc->out_fd);
}

int	exec(t_i_mode_vars *i_vars)
{
	int			i;
	t_proc_unit	*proc_list;
	t_proc_unit	*current_proc;
	char		**argv;
	t_list		*redirect_fds;

	proc_list = process_division(i_vars->token_list, &i_vars->pro_count);
	i_vars->child_pids = malloc(sizeof(pid_t) * i_vars->pro_count);
	// printf("process_divisionの後\n");
	// debug_put_proc_list(proc_list);
	i = -1;
	current_proc = proc_list;
	while (++i <= i_vars->pro_count && current_proc)
	{
		redirect_fds = NULL;
		redirect_fds = pipe_redirect(current_proc, redirect_fds);
		redirect_fds = open_and_redirect_files(current_proc->args,
				redirect_fds);
		i_vars->child_pids[i] = fork();
		if (i_vars->child_pids[i] == 0)
		{
			argv = tokens_to_arr(current_proc->args);
			argv = trim_redirection(&argv);
			get_command_path(&argv[0]);
			execve(argv[0], argv, __environ);
			perror("execve");
			return (EXIT_FAILURE);
		}
		else
		{
			reset_redirection(redirect_fds);
      close_pipe(current_proc);
		}
		current_proc = current_proc->next;
	}
	return (0);
}

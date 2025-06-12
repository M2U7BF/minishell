/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:47:09 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/12 16:19:20 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// ヒアドキュメントの処理。
// 入力データはパイプによって、カーネルにバッファリングされる。
int	here_doc(char *delimiter)
{
	char	*line;
	int		pipe_fds[2];

	pipe(pipe_fds);
	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			ft_free(line);
			break ;
		}
		ft_dprintf(pipe_fds[1], "%s\n", line);
		ft_free(line);
	}
	close(pipe_fds[1]);
	return (pipe_fds[0]);
}

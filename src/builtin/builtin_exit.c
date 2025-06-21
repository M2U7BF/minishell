/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:50 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/20 16:39:55 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// exit　完成？
/*todo; オプションはないが、終了コードは渡す(exit 42)*/
void	builtin_exit(char **argv)
{
	ft_putendl_fd("exit_buitin", STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
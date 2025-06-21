/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:47 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/20 15:48:26 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// env 仮実装
// getenv()？　mainのきれいな修正案を思いつかないので一旦これ
void	builtin_env(void)
{
	extern	char	**environ; //グローバル変数(2つ目)（要修正）
	int			i;

	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], STDOUT_FILENO);
		i++;
	}
	ft_putendl_fd("ビルトインです", STDOUT_FILENO);
}

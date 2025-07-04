/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:57 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/04 11:54:16 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(char **argv)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: unset: `%s': not a valid identifier\n", argv[i]);
			status = 1;
		}
		else
		{
			unset_env_var(&g_vars.env_list, argv[i]);
		}
		i++;
	}
	return (status);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "../../include/minishell.h"

// char **environ;

// void print_environ(const char *title)
// {
// 	int i = 0;
// 	printf("---- %s ----\n", title);
// 	while (environ[i])
// 	{
// 		printf("[%d] %s\n", i, environ[i]);
// 		i++;
// 	}
// 	printf("--------------------\n");
// }

// // テスト用に環境変数を初期化する
// void init_test_environ(void)
// {
// 	environ = malloc(sizeof(char *) * 5);
// 	environ[0] = strdup("PATH=/usr/bin");
// 	environ[1] = strdup("HOME=/home/user");
// 	environ[2] = strdup("SHELL=/bin/bash");
// 	environ[3] = strdup("USER=tester");
// 	environ[4] = NULL;
// }

// int main(void)
// {
// 	char *argv1[] = {"unset", "HOME", "INVALID-NAME", NULL};
// 	char *argv2[] = {"unset", "USER", "PATH", NULL};

// 	init_test_environ();
// 	print_environ("Before unset");

// 	printf("Running builtin_unset(argv1):\n");
// 	builtin_unset(argv1);
// 	print_environ("After unset argv1");

// 	printf("Running builtin_unset(argv2):\n");
// 	builtin_unset(argv2);
// 	print_environ("After unset argv2");

// 	// メモリ解放
// 	for (int i = 0; environ[i]; i++)
// 		free(environ[i]);
// 	free(environ);

// 	return (0);
// }

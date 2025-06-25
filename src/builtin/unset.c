/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:57 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/25 16:35:21 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern char	**environ;

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

int builtin_unset(char **argv)
{
	int		old_count;
	char	**new_environ;
	int		i;
	int		j;
	int		status;

	status = 0;
	i = 1;
	while(argv[i]) // 先に引数のバリデーションを行う
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_dprintf(STDERR_FILENO, "minishell: unset: `%s': not a valid identifier\n", argv[i]);
			status = 1;
		}
		i++;
	}

	old_count = 0;
	while (environ[old_count])
		old_count++;

	// 新しい配列を確保 (最大で元のサイズと同じ)
	new_environ = ft_calloc(old_count + 1, sizeof(char *));
	if (!new_environ)
		return (1);

	i = 0;
	j = 0;
	while (environ[i])
	{
		int		k = 1;
		int		should_unset = 0;
		while (argv[k])
		{
			size_t len = ft_strlen(argv[k]);
			if (is_valid_identifier(argv[k]) && ft_strncmp(environ[i], argv[k], len) == 0 && environ[i][len] == '=')
			{
				should_unset = 1;
				break;
			}
			k++;
		}
		if (should_unset)
			free(environ[i]); // 削除対象の文字列を解放
		else
			new_environ[j++] = environ[i]; // 保持する変数のポインタを新しい配列にコピー
		i++;
	}
	new_environ[j] = NULL;
	free(environ); // 古いポインタ配列を解放
	environ = new_environ; // グローバルポインタを新しい配列に差し替える
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

// 	return 0;
// }

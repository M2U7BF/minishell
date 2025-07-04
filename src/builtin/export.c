/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:52 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/04 11:54:10 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_identifier(const char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	s++;
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

static void	print_sorted_env(t_list *env_list)
{
	char	**env_array;

	env_array = convert_env_list_to_array(env_list);
	// int count = arrlen(env_array);
	free_str_array(env_array);
}

int	builtin_export(char **argv)
{
	int		i;
	int		status;
	char	*key;
	char	*value;
	char	*eq_ptr;

	i = 1;
	status = 0;
	if (argv[1] == NULL)
	{
		print_sorted_env(g_vars.env_list); // 引数を渡す
		return (0);
	}
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n", argv[i]);
			status = 1;
			i++;
			continue ;
		}
		eq_ptr = ft_strchr(argv[i], '=');
		if (eq_ptr) // "key=value" 形式
		{
			key = ft_substr(argv[i], 0, eq_ptr - argv[i]);
			value = eq_ptr + 1;
			set_env_var(&g_vars.env_list, key, value);
			free(key);
		}
		else
		{
			set_env_var(&g_vars.env_list, argv[i], "");
		}
		i++;
	}
	return (status);
}

//----------------------------------------------------------------------
// char **make_argv(const char *args[], int count)
// {
// 	char **argv = malloc(sizeof(char *) * (count + 1));
// 	int i = 0;

// 	if (!argv)
// 		return (NULL);
// 	while (i < count)
// 	{
// 		argv[i] = strdup(args[i]);
// 		i++;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }

// void free_argv(char **argv)
// {
// 	int i = 0;
// 	while (argv[i])
// 		free(argv[i++]);
// 	free(argv);
// }

// int main(void)
// {
// 	// テスト1: 引数なし（環境変数一覧表示）
// 	printf("=== Test 1: No arguments (should print all envs) ===\n");
// 	char *args1[] = { "export", NULL };
// 	char **argv1 = make_argv(args1, 1);
// 	builtin_export(argv1);
// 	free_argv(argv1);

// 	// テスト2: 有効な変数の追加
// 	printf("\n=== Test 2: Valid variable ===\n");
// 	char *args2[] = { "export", "MYVAR1=value1", NULL };
// 	char **argv2 = make_argv(args2, 2);
// 	builtin_export(argv2);
// 	free_argv(argv2);

// 	// テスト3: 無効な識別子
// 	printf("\n=== Test 3: Invalid identifier ===\n");
// 	char *args3[] = { "export", "1INVALID=value", NULL };
// 	char **argv3 = make_argv(args3, 2);
// 	builtin_export(argv3);
// 	free_argv(argv3);

// 	// テスト4: 複数の変数
// 	printf("\n=== Test 4: Multiple variables ===\n");
// 	char *args4[] = { "export", "VAR_A=123", "VAR_B=456", "_VALID=ok", NULL };
// 	char **argv4 = make_argv(args4, 4);
// 	builtin_export(argv4);
// 	free_argv(argv4);

// 	// 最後に確認：すべての環境変数
// 	printf("\n=== Final Environment Variables ===\n");
// 	char *args5[] = { "export", NULL };
// 	char **argv5 = make_argv(args5, 1);
// 	builtin_export(argv5);
// 	free_argv(argv5);

// 	return (0);
// }

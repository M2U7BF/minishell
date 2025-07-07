/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:52 by atashiro          #+#    #+#             */
/*   Updated: 2025/07/07 17:55:23 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_env_entry(const char *entry)
{
	char	*eq_ptr;
	char	*key;
	char	*value;

	eq_ptr = ft_strchr(entry, '=');
	if (eq_ptr)
	{
		key = ft_substr(entry, 0, eq_ptr - entry);
		value = eq_ptr + 1;
		ft_dprintf(STDOUT_FILENO, "declare -x %s=\"%s\"\n", key, value);
		free(key);
	}
	else
	{
		ft_dprintf(STDOUT_FILENO, "declare -x %s\n", entry);
	}
}

static void	print_sorted_env(t_list *env_list)
{
	char	**env_array;
	int		i;

	env_array = convert_env_list_to_array(env_list);
	if (!env_array)
		return ;
	sort_env_array(env_array);
	i = 0;
	while (env_array[i])
	{
		print_env_entry(env_array[i]);
		i++;
	}
	free_str_array(env_array);
}

static int	handle_invalid_identifier(const char *arg)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: export: `%s': not a valid identifier\n", arg);
	return (1);
}

static void	handle_valid_export(const char *arg)
{
	char	*eq_ptr;
	char	*key;
	char	*value;

	eq_ptr = ft_strchr(arg, '=');
	if (eq_ptr)
	{
		key = ft_substr(arg, 0, eq_ptr - arg);
		value = eq_ptr + 1;
		set_env_var(&g_vars.env_list, key, value);
		free(key);
	}
	else
	{
		set_env_var(&g_vars.env_list, arg, "");
	}
}

int	builtin_export(char **argv)
{
	int	i;
	int	status;

	if (argv[1] == NULL)
	{
		print_sorted_env(g_vars.env_list);
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!is_valid_export(argv[i]))
			status |= handle_invalid_identifier(argv[i]);
		else
			handle_valid_export(argv[i]);
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

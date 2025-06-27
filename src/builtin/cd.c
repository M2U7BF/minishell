/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:44 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/23 13:49:11 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	update_env_vars(void);
static int	change_dir_and_update_env(const char *path);
static int	update_env_vars(void)
{
	char	cwd[1000];
	char	*old_pwd;

	old_pwd = getenv("PWD");
	if (old_pwd != NULL)
	{
		if (setenv("OLDPWD", old_pwd, 1) != 0)
		{
			perror("minishell: cd");
			return (1);
		}
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd");
		return (1);
	}
	if (setenv("PWD", cwd, 1) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

static int	change_dir_and_update_env(const char *path)
{
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (update_env_vars());
}

int	builtin_cd(char **argv)
{
	char	*path;
	int		argc;

	argc = arrlen(argv);
	if (argc > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (argc == 1 || (argc > 1 && ft_strncmp(argv[1], "~", 2) == 0))
	{
		path = getenv("HOME");
		if (path == NULL || *path == '\0')
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else if (argc > 1 && ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = getenv("OLDPWD");
		if (path == NULL)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
			return (1);
		}
		ft_putendl_fd(path, STDOUT_FILENO);
	}
	else
	{
		path = argv[1];
	}
	return (change_dir_and_update_env(path));
}

//--------------test---------------

// int arrlen(char **argv)
// {
// 	int i = 0;
// 	while (argv[i])
// 		i++;
// 	return i;
// }


// void print_current_directory(void)
// {
// 	char cwd[1024];
// 	if (getcwd(cwd, sizeof(cwd)))
// 		printf("Current directory: %s\n", cwd);
// 	else
// 		perror("getcwd");
// }

// int main(void)
// {
// 	char *argv1[] = {"cd", NULL};
// 	char *argv2[] = {"cd", "..", NULL};
// 	char *argv3[] = {"cd", "-", NULL};
// 	char *argv4[] = {"cd", "/tmp", NULL};
// 	char *argv5[] = {"cd", "too", "many", NULL};

// 	printf("=== Test 1: cd (no argument) ===\n");
// 	print_current_directory();
// 	builtin_cd(argv1);
// 	print_current_directory();

// 	printf("\n=== Test 2: cd .. ===\n");
// 	builtin_cd(argv2);
// 	print_current_directory();

// 	printf("\n=== Test 3: cd - (go to OLDPWD) ===\n");
// 	builtin_cd(argv3);
// 	print_current_directory();

// 	printf("\n=== Test 4: cd /tmp ===\n");
// 	builtin_cd(argv4);
// 	print_current_directory();

// 	printf("\n=== Test 5: cd with too many arguments ===\n");
// 	builtin_cd(argv5);
// 	print_current_directory();

// 	return 0;
// }


 //cc cd.c ../../lib/ft_dprintf/libftdprintf.a ../../lib/libft/libft.a
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atashiro <atashiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:44:44 by atashiro          #+#    #+#             */
/*   Updated: 2025/06/30 14:19:59 by atashiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	update_env_vars(t_list *env_list)
{
	char	cwd[1024];
	char	*old_pwd;

	old_pwd = get_env_value(env_list, "PWD");
	if (old_pwd != NULL)
	{
		set_env_var(&env_list, "OLDPWD", old_pwd);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd");
		return (1);
	}
	set_env_var(&env_list, "PWD", cwd);
	return (0);
}

static int	change_dir_and_update_env(const char *path, t_list *env_list)
{
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (update_env_vars(env_list));
}

int	builtin_cd(char **argv, t_list *env_list)
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
		path = get_env_value(env_list, "HOME");
		if (path == NULL || *path == '\0')
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else if (argc > 1 && ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = get_env_value(env_list, "OLDPWD");
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
	return (change_dir_and_update_env(path, env_list));
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
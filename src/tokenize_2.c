/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:29:34 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/03 10:54:24 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_metacharacter(char *s)
{
	int			i;
	static char	*chars[] = {"|", "&", ";", "(", ")", "<", ">", " ", "\t", "\v"};
	static int	len = sizeof(chars) / sizeof(chars[0]);

	if (!s)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

bool	is_word(char *s)
{
	return (*s && !is_metacharacter(s));
}

bool	is_control_operator(char *s)
{
	int			i;
	static char	*opes[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "|&",
		"\n"};
	static int	len = sizeof(opes) / sizeof(opes[0]);

	if (!s)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, opes[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

bool	is_reserved_word(char *s)
{
	int			i;
	static char	*words[] = {"!", "case", "do", "done", "elif", "else", "esac",
		"fi", "for", "function", "if", "in", "select", "then", "until",
		"while", "{", "}", "time", "[[", "]]"};
	static int	len = sizeof(words) / sizeof(words[0]);

	if (!s)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (ft_strncmp(s, words[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

bool	is_redirection(char *s)
{
	int			i;
	static char	*chars[] = {">>", "<<", ">", "<", NULL};

	if (!s)
		return (false);
	i = -1;
	while (chars[++i])
	{
		if (ft_strncmp(s, chars[i], ft_strlen(s) + 1) == 0)
			return (true);
	}
	return (false);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 08:30:54 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/12 11:42:24 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	*free_ptr(char **p)
{
	free(*p);
	*p = NULL;
	return (NULL);
}

static char	*read_into(int fd, char **str, char *buf, int *bytes)
{
	*bytes = read(fd, buf, BUFFER_SIZE);
	if (*bytes == -1)
		return (free_ptr(str));
	if (*bytes != 0)
	{
		buf[*bytes] = '\0';
		if (!*str)
			*str = ft_strdup(buf);
		else
			*str = ft_strjoin(*str, buf);
		if (!*str)
			return (NULL);
	}
	return (*str);
}

static char	*get_one_line(char **str)
{
	char	*result;
	char	*old_ptr;

	result = NULL;
	if (!*str)
		return (NULL);
	else if (ft_strchr(*str, '\n'))
	{
		result = ft_substr(*str, 0, ft_strchr(*str, '\n') - *str + 1);
		if (!result)
			return (free_ptr(str));
		old_ptr = *str;
		*str = ft_strdup(ft_strchr(*str, '\n') + 1);
		free(old_ptr);
		if (!*str)
			return (free_ptr(&result));
	}
	else
	{
		if ((*str)[0])
			result = ft_strdup(*str);
		free_ptr(str);
	}
	return (result);
}

char	*get_next_line(int fd, int free_all)
{
	static char	*str = NULL;
	int			bytes;
	char		*buf;

	if (free_all)
		return (free_ptr(&str), NULL);
	if (BUFFER_SIZE <= 0 || BUFFER_SIZE + 1 > SIZE_MAX)
		return (free_ptr(&str));
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (free_ptr(&str));
	bytes = 1;
	while (bytes > 0)
	{
		str = read_into(fd, &str, buf, &bytes);
		if (!str)
		{
			free(buf);
			return (NULL);
		}
		if (ft_strchr(str, '\n') != NULL)
			break ;
	}
	free(buf);
	return (get_one_line(&str));
}

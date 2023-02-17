/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwankim <hwankim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 19:47:07 by hwankim           #+#    #+#             */
/*   Updated: 2023/02/14 16:36:15 by hwankim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	ft_close(int fd, int fd2)
{
	close(fd);
	close(fd2);
}

void	ft_dup2(int fd, int fd2)
{
	dup2(fd, STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
	ft_close(fd, fd2);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while ((*s1 || *s2) && n > 0)
	{
		if (*s1 == *s2)
		{
			s1++;
			s2++;
			n--;
		}
		else
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
	}
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 == *s2)
		{
			s1++;
			s2++;
		}
		else
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
	}
	return (0);
}

void	here_doc(t_files *files, char *argv[])
{
	char	*line;

	if (pipe(files->write_pi) == -1)
		ft_perror("pipe error", 1);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
			line[ft_strlen(line) - 1] = '\0';
		if (!line || !ft_strcmp(line, argv[2]))//here_doc 뒤에 특정문자열을 입력하면 표준입력을 gnl로 읽어오면서 특정문자열과 똑같은 문자열이 등장하면 멈춘다.
		{
			free(line);
			break ;
		}
		line[ft_strlen(line)] = '\n';//특정 문자열이 아니면 \n 넣어주고
		write(files->write_pi[WRITE], line, ft_strlen(line));//특정 문자열이 아니면 write파이프에 넣어주기
		free(line);
	}
	files->read_pi[0] = files->write_pi[0];//read파이프의 출구로 접근하면 write파이프의 정보를 읽어옴
	close(files->write_pi[WRITE]);
	files->cmd_num = 3;
}

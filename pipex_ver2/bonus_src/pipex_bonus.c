/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwankim <hwankim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 22:10:06 by hwankim           #+#    #+#             */
/*   Updated: 2023/02/14 16:36:15 by hwankim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

char	*get_vaild_cmd(char **path, char *cmd)
{
	int		i;
	char	*temp_address;
	char	*path_plus_cmd;
	char	*temp_cmd;

	i = 0;
	path_plus_cmd = NULL;
	if (access(cmd, X_OK) == 0)
		return (cmd);
	temp_cmd = ft_strjoin("/", cmd);
	if (!temp_cmd)
		exit (1);
	while (path[i])
	{
		temp_address = path_plus_cmd;
		path_plus_cmd = ft_strjoin(path[i], temp_cmd);
		free (temp_address);
		if (access(path_plus_cmd, X_OK) == 0)
			return (path_plus_cmd);
		i++;
	}
	return (NULL);
}

void	get_path(t_files *files, char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH", 4))
		{
			envp[i] += 5;
			break ;
		}
		i++;
	}
	files->path = ft_split(envp[i], ':');
}

void	execute(t_files *files, char *argv[], char *envp[])
{
	files->cmd_options = ft_split(argv[files->cmd_num], ' ');//cmd_num는 프로그램 실행시 각 명령어에 대응되는 수
	if (!files->cmd_options)
		ft_perror("empty string", 1);
	files->cmd = get_vaild_cmd(files->path, files->cmd_options[0]);//파일경로 + 파일명을 반환
	if (!files->cmd)
		ft_perror("not valid cmd", 1);
	if (files->cmd_num == 2)//here_doc을 쓰지 않을 때
	{
		ft_dup2(files->infile, files->write_pi[WRITE]);
		execve(files->cmd, files->cmd_options, envp);
	}
	else if (files->cmd_num == files->argc - 2)//마지막 명령어 일때
	{
		ft_dup2(files->read_pi[READ], files->outfile);
		execve(files->cmd, files->cmd_options, envp);
	}
	else//here_doc쓸때
	{
		ft_dup2(files->read_pi[READ], files->write_pi[WRITE]);
		execve(files->cmd, files->cmd_options, envp);
	}
}

void	run_command(t_files *files, char *argv[], char *envp[])
{
	int	fd;

	while (files->cmd_num < files->argc - 1)//
	{
		if (files->cmd_num != files->argc - 2)//마지막 명령일 때는 write파이프 필요없음 outfile에 출력을 담을거라서
		{
			if (pipe(files->write_pi) == -1)//write파이프는 자식프로세스에서 실행한 명령어의 출력을 담는 역할...명령어는 전 write파이프에서 입력을 받고 후 write 파이프에 출력을 담는다.
				ft_perror("pipe error", 1);
		}
		files->pid = fork();
		if (files->pid == -1)
			ft_perror("fork error", 1);
		else if (files->pid == 0)//자식프로세스 섹션
			execute(files, argv, envp);
		else
		{
			wait(0);
			close(files->write_pi[WRITE]);
			fd = files->read_pi[READ];//here_doc으로 받았던 내용을 담은 파이프의 출구
			files->read_pi[READ] = files->write_pi[READ];//자식에서 write파이프에 담은정보를 read_fd[READ]가 가르키게하고
			close(fd);                                   //새로운 자식에서 read_fd[READ]를 입력으로 받는다.
			files->cmd_num++;
		}
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_files	files;

	if (argc < 5)
		exit(1);
	init(&files, argc, argv, envp);
	run_command(&files, argv, envp);
	exit(0);
}

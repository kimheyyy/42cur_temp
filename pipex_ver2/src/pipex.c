/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwankim <hwankim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 22:24:44 by hwankim           #+#    #+#             */
/*   Updated: 2023/02/14 16:36:15 by hwankim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*get_vaild_cmd(char **path, char *cmd)
{
	int		i;
	char	*temp_address;
	char	*path_plus_cmd;
	char	*temp_cmd;

	i = 0;
	path_plus_cmd = NULL;
	if (access(cmd, X_OK) == 0)//명령어 파일 cmd가 실행할 수 있는지 확인
		return (cmd);
	temp_cmd = ft_strjoin("/", cmd);// /cmd로 만들어서 temp_cmd에 담자
	while (path[i])
	{
		temp_address = path_plus_cmd;
		path_plus_cmd = ft_strjoin(path[i], temp_cmd);//환경변수 경로에서 명령어를 합쳐주자
		free (temp_address);
		if (access(path_plus_cmd, X_OK) == 0)//환경변수+명령어가 실행할 수 있는지 체크
		{
			free(temp_cmd);
			return (path_plus_cmd);
		}
		i++;
	}
	free(temp_cmd);
	return (NULL);
}

void	get_path(t_files *files, char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH", 4))//4만큼 비교했을때 앞에 PATH라는 문자가 있으면
		{
			envp[i] += 5;
			break ;
		}
		i++;
	}
	files->path = ft_split(envp[i], ':');//:를 구분자로 잘라서 이차원 문자배열에 담기
}

void	init(t_files *files, char *argv[], char *envp[])
{
	files->infile = open(argv[1], O_RDONLY);//인자로 받으면 파일열기 ->infile:첫번쨰 디스크립터번호
	if (files->infile == -1)
		ft_perror(argv[1], 1);
	files->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);//4번쨰 파일 디스크립터 번호 받아오기
	if (files->outfile == -1)
		ft_perror(argv[4], 1);
	get_path(files, envp);//환경변수를 files->path에 담아주는 함수
	files->cmd_options = ft_split(argv[2], ' ');//옵션 같은게 있을수도 있으니 빈공간을 구분자로 해서 명령어 담기
	files->cmd_options2 = ft_split(argv[3], ' ');//마찬가지
	if (!files->cmd_options || !files->cmd_options2)
		ft_perror("empty string", 1);
	//존재하는 명령어면 경로+명령어가 담기고 아니면 null이 담김
	files->f_cmd = get_vaild_cmd(files->path, files->cmd_options[0]);//명령어가 존재하는지 확인해주는 함수(환경변수 경로+명령어를 반복문을 통해서 전부 확인)
	files->s_cmd = get_vaild_cmd(files->path, files->cmd_options2[0]);
	if (!files->f_cmd || !files->s_cmd)//명령어중 존재하지 않는게 있다면 바로 에러
		ft_perror("not valid cmd", 1);
}

void	run_command(t_files files, char *envp[])
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)//파이프 생성
		ft_perror("pipe error", 1);
	pid = fork();//자식 프로세서 생성
	if (pid == -1)
		ft_perror("fork error", 1);
	else if (pid > 0)//부모 프로세서 파트
	{
		close(fd[WRITE]);
		ft_dup2(fd[READ], files.outfile);//부모 프로세서의 표준입력이 fd[READ]를,표준출력이 outfile을 가르키게 한다
		wait(0);
		execve(files.s_cmd, files.cmd_options2, envp);
	}
	else//자식 프로세서 파트
	{
		close(fd[READ]);
		ft_dup2(files.infile, fd[WRITE]);//자식 프로세서의 표준입력이 infile을,자식 프로세스의 표준출력이 fd[WRITE]를
		execve(files.f_cmd, files.cmd_options, envp);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_files	files;

	if (argc != 5)//입력값 4개아니면 종료
		exit(1);
	init(&files, argv, envp);//환경변수 담아서 명령어 파일이 존재하는지 확인한다.
	run_command(files, envp);//자식프로세스->cmd1,부모프로세스->cmd2 실행
}

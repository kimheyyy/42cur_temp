/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwankim <hwankim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 22:12:31 by hwankim           #+#    #+#             */
/*   Updated: 2023/02/14 16:36:15 by hwankim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	init(t_files *files, int argc, char *argv[], char *envp[])
{
	files->argc = argc;
	files->cmd_num = 2;
	if (pipe(files->read_pi) == -1)
		ft_perror("pipe error", 1);
	close(files->read_pi[WRITE]);
	get_path(files, envp);
	if (!ft_strcmp(argv[1], "here_doc"))//실행파일 사용자가 입력한 1번째 인자가 "here_doc"이면
	{
		files->outfile = open(argv[argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (files->outfile == -1)
			ft_perror(argv[argc - 1], 1);
		here_doc(files, argv);
	}
	else
	{
		files->infile = open(argv[1], O_RDONLY);
		if (files->infile == -1)
			ft_perror(argv[1], 1);
		files->outfile = open(argv[argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (files->outfile == -1)
			ft_perror(argv[argc - 1], 1);
	}
}

void	ft_perror(char *msg, int exit_status)
{
	perror(msg);
	exit(exit_status);
}

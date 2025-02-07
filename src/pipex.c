/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimagalh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:09:09 by rimagalh          #+#    #+#             */
/*   Updated: 2025/01/15 15:09:09 by rimagalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	*find_cmd(char *cmd, char **envp)
{
	char	*temp;
	char	**paths;
	char	*valid;
	int		i;

	i = 0;
	temp = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			temp = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!temp)
		return (NULL);
	paths = ft_split(temp, ':');
	if (paths == NULL)
		return (NULL);
	valid = valid_path(paths, cmd);
	free_mem(paths, NULL);
	return (valid);
}

static void	do_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	if (*cmd == '\0')
	{
		ft_printf("command not found");
		exit(127);
	}
	args = ft_split(cmd, ' ');
	if (args == NULL || args[0] == NULL)
	{
		perror("command not found");
		free_mem(args, path);
		exit(127);
	}
	path = find_cmd(args[0], envp);
	if (path == NULL)
	{
		perror("command not found");
		free_mem(args, path);
		exit(127);
	}
	execve(path, args, envp);
	perror("command not found");
	return (exit(127), free_mem(args, path));
}

static void	exec_first(int fd_in, int *fd_pipe, char**argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(fd_pipe[0]);
		dup2(fd_in, 0);
		dup2(fd_pipe[1], 1);
		close_files(fd_pipe[1], fd_in, -1, -1);
		do_cmd(argv[2], envp);
		exit(0);
	}
}

static void	exec_second(int fd_out, int *fd_pipe, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], 0);
		dup2(fd_out, 1);
		close_files(fd_pipe[0], fd_out, -1, -1);
		do_cmd(argv[3], envp);
		exit(0);
	}
}

void	pipex(char **argv, char **envp)
{
	int		fd_in;
	int		fd_out;
	int		fd_pipe[2];

	open_files(argv, &fd_in, &fd_out);
	if (pipe(fd_pipe) < 0)
	{
		perror("Pipe error\n");
		close_files(fd_in, fd_out, -1, -1);
		exit(1);
	}
	exec_first(fd_in, fd_pipe, argv, envp);
	exec_second(fd_out, fd_pipe, argv, envp);
	close_files(fd_pipe[0], fd_pipe[1], fd_in, fd_out);
	wait(NULL);
	wait(NULL);
	exit(0);
}

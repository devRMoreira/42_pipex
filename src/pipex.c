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

//* infile "grep a1" "wc -w" outfile

static void test_files(char* infile, char* outfile)
{
	int fd;

	fd = open(infile, O_RDONLY);
	if(fd < 0)
	{
		ft_printf("infile error.\n");
		exit(1);
	}
	close(fd);
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd < 0)
	{
		ft_printf("outfile error.\n");
		exit(1);
	}
	close(fd);
}

static char	*valid_path(char **paths, char *cmd)
{
	int i;
	int size;
	char *temp;

	i = 0;
	size = ft_strlen(cmd) + ft_strlen(paths[i]) + 2;
	while(paths[i])
	{
		temp = malloc(sizeof(char) * size);
		if(!temp)
			return (NULL);
		ft_strlcpy(temp, paths[i], size);
		ft_strlcat(temp, "/", size);
		ft_strlcat(temp, cmd, size);
		if(access(temp, X_OK) == 0)
			return (temp);
		free(temp);
		i++;
	}

	return (NULL);
}

static char *find_cmd(char *cmd, char **envp)
{
	char *temp;
	char **paths;
	char *valid;
	int i;

	i = 0;
	temp = NULL;
	while(envp[i])
	{
		if(ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			temp = envp[i] + 5;
			break;
		}
		i++;
	}
	if(!temp)
		return (NULL);

	paths = ft_split(temp, ':');
	valid = valid_path(paths, cmd);
	free(paths);
	return (valid);
}

static void do_cmd(char* cmd, int input_fd, int output_fd, char **envp)
{
	pid_t	pid;
	char	**args;
	char	*path;

	pid = fork();
	args = ft_split(cmd, ' ');
	path = find_cmd(args[0], envp);

	if(!path)
	{
		perror("Invalid command");
		exit(1);
	}

	if(pid < 0)
	{
		perror("Fork error");
		exit(1);
	}

	if(pid == 0)
	{
		if(dup2(input_fd, 0) < 0)
		{
			perror("dup2 input error");
			exit(1);
		}
		if(dup2(output_fd, 1) < 0)
		{
			perror("dup2 input error");
			exit(1);
		}

		close(input_fd);
		close(output_fd);
		execve(path, args, envp);
		perror("Command exec failure");
		exit(1);
	}
	else
	{
		close(input_fd);
		close(output_fd);
	}

}

void pipex(char* infile, char* cmd1, char* cmd2, char* outfile, char **envp)
{
	int fd_in;
	int fd_out;
	int fd_pipe[2];

	test_files(infile, outfile);
	fd_in = open(infile, O_RDONLY);
	fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if(pipe(fd_pipe) < 0)
	{
		perror("Pipe error\n");
		close(fd_in);
		close(fd_out);
		exit(1);
	}
	do_cmd(cmd1, fd_in, fd_pipe[1], envp);
	close(fd_pipe[1]);
	
	do_cmd(cmd2, fd_out, fd_pipe[0], envp);
	close(fd_pipe[0]);
	close(fd_out);

	wait(NULL);
	wait(NULL);
}

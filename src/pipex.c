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

static void	free_mem(char **split, char *str)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	free(str);
}

static char	*valid_path(char **paths, char *cmd)
{
	int		i;
	int		size;
	char	*temp;

	i = 0;
	size = ft_strlen(cmd) + ft_strlen(paths[i]) + 2;
	while (paths[i])
	{
		temp = malloc(sizeof(char) * size);
		if (!temp)
			return (NULL);
		ft_strlcpy(temp, paths[i], size);
		ft_strlcat(temp, "/", size);
		ft_strlcat(temp, cmd, size);
		if (access(temp, X_OK) == 0)
		{
			free_mem(paths, NULL);
			return (temp);
		}
		free(temp);
		i++;
	}
	free_mem(paths, NULL);
	return (NULL);
}

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
	valid = valid_path(paths, cmd);
	return (valid);
}

static void	do_cmd(char *cmd, int input_fd, int output_fd, char **envp)
{
	pid_t	pid;
	char	**args;
	char	*path;

	pid = fork();
	args = ft_split(cmd, ' ');
	path = find_cmd(args[0], envp);
	if (pid == 0)
	{
		dup2(input_fd, 0);
		dup2(output_fd, 1);
		execve(path, args, envp);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
	{
		close(input_fd);
		close(output_fd);
	}
	free_mem(args, path);
}

void	pipex(char **argv, char **envp)
{
	int	fd_in;
	int	fd_out;
	int	fd_pipe[2];

	fd_in = open(argv[1], O_RDONLY);
	if (fd_in < 0)
	{
		perror("infile error.\n");
		exit(1);
	}
	fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		perror("outfile error.\n");
		close(fd_in);
		exit(1);
	}
	if (pipe(fd_pipe) < 0)
	{
		perror("Pipe error\n");
		close(fd_in);
		close(fd_out);
		exit(1);
	}
	do_cmd(argv[2], fd_in, fd_pipe[1], envp);
	close(fd_pipe[1]);
	do_cmd(argv[3], fd_pipe[0], fd_out, envp);
	close(fd_pipe[0]);
	close(fd_out);
	wait(NULL);
	wait(NULL);
}

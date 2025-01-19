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

static void	do_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	path = find_cmd(args[0], envp);
	execve(path, args, envp);
	perror("command not found");
	exit(127);
	free_mem(args, path);
}

void	pipex(char **argv, char **envp)
{
	int		fd_in;
	int		fd_out;
	int		fd_pipe[2];
	pid_t	pid1;
	pid_t	pid2;

	open_files(argv, &fd_in, &fd_out);

	if (pipe(fd_pipe) < 0)
	{
		perror("Pipe error\n");
		close(fd_in);
		close(fd_out);
		exit(1);
	}
    pid1 = fork();
    if (pid1 == 0)
    {
        close(fd_pipe[0]);
        dup2(fd_in, 0);
        dup2(fd_pipe[1], 1);
        close(fd_pipe[1]);
        close(fd_in);
        do_cmd(argv[2], envp);
        exit(0);
    }
    pid2 = fork();
    if (pid2 == 0)
    {
        close(fd_pipe[1]);
        dup2(fd_pipe[0], 0);
        dup2(fd_out, 1);
        close(fd_pipe[0]);
        close(fd_out);
        do_cmd(argv[3], envp);
        exit(0);
    }

    close(fd_pipe[0]);
    close(fd_pipe[1]);
    close(fd_in);
    close(fd_out);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    exit(0);
}

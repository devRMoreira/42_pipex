/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimagalh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:14:33 by rimagalh          #+#    #+#             */
/*   Updated: 2025/02/07 11:39:53 by rimagalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	open_files(char **argv, int *fd_in, int *fd_out)
{
	*fd_in = open(argv[1], O_RDONLY);
	if (*fd_in < 0)
	{
		perror("infile error.\n");
		exit(1);
	}
	*fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out < 0)
	{
		perror("outfile error.\n");
		close(*fd_in);
		exit(1);
	}
}

void	close_files(int fd1, int fd2, int fd3, int fd4)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	if (fd3 >= 0)
		close(fd3);
	if (fd4 >= 0)
		close(fd4);
}

void	free_mem(char **split, char *str)
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

char	*valid_path(char **paths, char *cmd)
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
			return (free_mem(paths, NULL), NULL);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimagalh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:14:33 by rimagalh          #+#    #+#             */
/*   Updated: 2025/01/19 15:23:24 by rimagalh         ###   ########.fr       */
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



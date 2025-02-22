/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimagalh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:50 by rimagalh          #+#    #+#             */
/*   Updated: 2025/02/13 11:11:50 by rimagalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>
# include <sys/wait.h>
# include "../lib/libft/libft.h"

void	pipex(char **argv, char **envp);
void	open_files(char **argv, int *fd_in, int *fd_out);
void	close_files(int fd1, int fd2, int fd3, int fd4);
void	free_mem(char **split, char *str);
char	*valid_path(char **paths, char *cmd);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimagalh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:58:31 by rimagalh          #+#    #+#             */
/*   Updated: 2025/01/07 15:58:31 by rimagalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>
# include <sys/wait.h>
# include "../lib/libft/libft.h"

void	pipex(char **argv, char **envp);
void	open_files(char **argv, int *fd_in, int *fd_out);

#endif

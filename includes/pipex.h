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

#include <sys/types.h>
#include <sys/wait.h>
#include "../lib/libft/libft.h"

void pipex(char* infile, char* cmd1, char* cmd2, char* outfile, char **envp);

#endif

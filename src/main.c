/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimagalh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:01:21 by rimagalh          #+#    #+#             */
/*   Updated: 2025/01/07 16:01:21 by rimagalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"


int	main(int argc, char **argv, char **envp)
{

	if(argc == 5)
	{
		pipex(argv[1], argv[2], argv[3], argv[4], envp);
	}
	else
	{
		ft_printf("Invalid arguments.\n");
		exit(1);
	}
	return (0);
}

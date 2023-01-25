/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 11:49:15 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/17 11:56:04 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_pwd(char **argv, char **envp)
{
	char	*path;

	(void)argv;
	(void)envp;
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd");
		return (-1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}

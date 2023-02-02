/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:20:48 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 15:08:30 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_system_error(char *line, char **envp, int fd)
{
	free(line);
	close(fd);
	rl_clear_history();
	free_all(envp);
	exit (1);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	char	**envp;
	int		fd;

	(void)argc;
	(void)argv;
	if (init_minishell(env, &envp, &fd, &line) != 0)
		return (-1);
	while (line)
	{
		if (line[0] != 0)
			add_to_history(fd, line);
		set_ics(TRUE);
		if (handle_cmd(line, &envp, TRUE) < 0)
			exit_system_error(line, envp, fd);
		set_ics(FALSE);
		free(line);
		line = readline("minishell> ");
	}
	printf("\033[1A\033[11Cexit\n");
	close(fd);
	rl_clear_history();
	free_all(envp);
	return (ft_atoi(get_les()));
}

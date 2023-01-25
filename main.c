/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:20:48 by lloison           #+#    #+#             */
/*   Updated: 2023/01/24 13:32:34 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Etat actuel :
 * Les redirections fonctionnent normalement entierement.
 * Les builtin fonctionnent correctement avec les redirection
 * Le code est normé
 */

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
		if (!is_cmd_name_empty(line))
			add_to_history(fd, line);
		set_ics(TRUE);
		if (handle_cmd(line, &envp) < 0)
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

// int	main(int argc, char **argv, char **env)
// {
// 	char	**envp;
// 	int		fd;

// 	(void)argc;
// 	(void)argv;
// 	if (init_minishell(env, &envp, &fd, &argv[2]) != 0)
// 		return (-1);
// 	set_ics(TRUE);
// 	if (handle_cmd(argv[2], &envp) < 0)
// 		exit_system_error(argv[1], envp, fd);
// 	set_ics(FALSE);
// 	if (!is_cmd_name_empty(argv[2]))
// 		add_to_history(fd, argv[2]);
// 	close(fd);
// 	rl_clear_history();
// 	free_all(envp);
// 	return (ft_atoi(get_les()));
// }
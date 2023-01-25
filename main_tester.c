/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tester.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 11:52:10 by lloison           #+#    #+#             */
/*   Updated: 2023/01/23 11:58:41 by lloison          ###   ########.fr       */
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
	char	**envp;
	int		fd;

	(void)argc;
	(void)argv;
	if (init_minishell(env, &envp, &fd, &argv[2]) != 0)
		return (-1);
	set_ics(TRUE);
	if (handle_cmd(argv[2], &envp) < 0)
		exit_system_error(argv[1], envp, fd);
	set_ics(FALSE);
	if (!is_cmd_name_empty(argv[2]))
		add_to_history(fd, argv[2]);
	close(fd);
	rl_clear_history();
	free_all(envp);
	return (ft_atoi(get_les()));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:46:30 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/26 16:36:16 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	filter_input_file(int ofd, char *end)
{
	char	*line;
	size_t	size;

	line = 0;
	size = ft_strlen(end);
	while (1)
	{
		line = readline("> ");
		if (line == 0)
			break ;
		if (ft_strncmp(line, end, size) == 0)
		{
			free(line);
			break ;
		}
		write(ofd, line, ft_strlen(line));
		write(ofd, "\n", 1);
		free(line);
	}
	return (0);
}

int	handle_here_doc(t_cmd *cmd, int i)
{
	pid_t	pid;
	int		fd[2];
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	cmd->iostream.input_file_fd = fd[0];
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		rl_catch_signals = 1;
		signal(SIGINT, overwrite_ctrl_c);
		if (filter_input_file(fd[1], cmd->args[i + 1]) == -1)
			exit(-1);
		close(fd[1]);
		exit(0);
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0)
		return (2);
	return (0);
}

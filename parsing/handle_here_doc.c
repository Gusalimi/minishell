/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:46:30 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/20 17:51:43 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	filter_input_file(int ofd, char *end)
{
	char	*line;
	char	*cmp;
	size_t	size;

	cmp = ft_strjoin(end, "\n");
	if (cmp == 0)
		return (-1);
	line = 0;
	size = ft_strlen(cmp);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (line == 0)
			break ;
		if (ft_strncmp(line, cmp, size) == 0)
		{
			free(line);
			break ;
		}
		write(ofd, line, ft_strlen(line));
		free(line);
	}
	free(cmp);
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
		signal(SIGINT, overwrite_ctrl_c);
		if (filter_input_file(fd[1], cmd->args[i + 1]) == -1)
			exit(-1);
		close(fd[1]);
		exit(0);
	}
	else
	{
		close(fd[1]);
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0)
			return (2);
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:00:43 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/22 12:44:57 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_history(int fd)
{
	char	*entry;
	char	*entry_no_nl;

	entry = get_next_line(fd);
	while (entry)
	{
		entry_no_nl = ft_strndup(entry, ft_strlen(entry) - 1);
		if (!entry_no_nl)
		{
			ft_printf_error("Unable to init the history.\n");
			return (-1);
		}
		add_history(entry_no_nl);
		free(entry);
		free(entry_no_nl);
		entry = get_next_line(fd);
	}
	return (0);
}

int	create_history(void)
{
	int		fd;

	if (access(".minishell_history", W_OK) == -1 && access(".minishell_history",
			F_OK) == 0)
	{
		ft_printf_error("Unable to create the history, if the problem");
		ft_printf_error(" persists, please try to delete .minishell_history\n");
		return (-1);
	}
	fd = open(".minishell_history", O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		ft_printf_error("Unable to create the history.\n");
		return (-1);
	}
	if (init_history(fd) == -1)
		return (-1);
	return (fd);
}

void	add_to_history(int fd, char *line)
{
	add_history(line);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

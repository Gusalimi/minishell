/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:00:43 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/26 15:22:56 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_history(int fd)
{
	char	*entry;
	char	*entry_no_nl;

	if (fd <= 0)
		return (0);
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

	fd = open(".minishell_history", O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		return (0);
	if (init_history(fd) == -1)
		return (-1);
	return (fd);
}

void	add_to_history(int fd, char *line)
{
	add_history(line);
	if (fd <= 0)
		return ;
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

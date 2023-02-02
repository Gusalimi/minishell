/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:57 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 16:57:35 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_cmds(t_cmd **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		free_all(cmds[i]->args);
		free(cmds[i]->path);
		if (cmds[i]->iostream.output_file_fd != 0)
			close(cmds[i]->iostream.output_file_fd);
		if (cmds[i]->iostream.input_file_fd != 0)
			close(cmds[i]->iostream.input_file_fd);
		if (cmds[i]->iostream.input_pipe[0] != 0)
			close(cmds[i]->iostream.input_pipe[0]);
		if (cmds[i]->iostream.input_pipe[1] != 0)
			close(cmds[i]->iostream.input_pipe[1]);
		free(cmds[i]);
		i++;
	}
	free(cmds);
	return (NULL);
}

static void	clean_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && is_ending_quote(line, i))
		{
			skip_quotes(line, &i);
			continue ;
		}
		if (line[i] == '#')
		{
			while (line[i])
			{
				line[i] = 0;
				i++;
			}
			return ;
		}
		i++;
	}
}

int	handle_cmd(char *line, char ***env, t_bool check_valid)
{
	t_cmd	**cmds;

	if (check_valid)
		clean_line(line);
	if (is_cmd_name_empty(line))
		return (0);
	if (check_valid && !is_line_valid(line))
	{
		ft_printf_error("minishell: syntax error\n");
		set_les(258);
		return (1);
	}
	cmds = parse_line(line);
	if (!cmds)
		return (-1);
	if (start_executing_cmds(cmds, env) == -1)
	{
		free_cmds(cmds);
		return (-1);
	}
	free_cmds(cmds);
	return (0);
}

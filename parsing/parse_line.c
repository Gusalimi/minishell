/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:00:14 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 15:43:50 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_breakpoint_type	get_next_breakpoint(char *line, int *i)
{
	while (is_space(line[*i]))
		*i = *i + 1;
	if (!line[*i])
		return (br_none);
	if (line[*i] == '|' && line[*i + 1] != '|')
	{
		*i = *i + 1;
		return (br_pipe);
	}
	if (line[*i] == '|' && line[*i + 1] == '|')
	{
		*i = *i + 2;
		return (br_or);
	}
	if (line[*i] == '&')
	{
		*i = *i + 2;
		return (br_and);
	}
	return (br_none);
}

static t_cmd	*init_cmd2(t_cmd *output, char *line, char **env, int *line_i)
{
	if (!output->is_builtin && !output->is_subshell)
	{
		output->path = get_cmd_path(output->name, env);
		if (!output->path)
		{
			free_all(output->args);
			free(output);
			return (NULL);
		}
	}
	output->next_breakpoint = get_next_breakpoint(line, line_i);
	return (output);
}

static t_cmd	*init_cmd(char *line, char **env, int *line_i)
{
	t_cmd	*output;

	output = ft_calloc(sizeof(t_cmd), 1);
	if (!output)
		return (NULL);
	output->args = get_cmd_args(line, line_i, FALSE);
	if (!output->args)
	{
		free(output);
		return (NULL);
	}
	output->name = get_cmd_name(output->args, line);
	output->is_builtin = is_cmd_builtin(output->name);
	output->is_subshell = is_cmd_subshell(output->name);
	return (init_cmd2(output, line, env, line_i));
}

t_cmd	**parse_line(char *line, char **env)
{
	int		nb_cmds;
	t_cmd	**cmds;
	int		cmd_i;
	int		line_i;

	cmd_i = 0;
	line_i = 0;
	nb_cmds = get_nb_cmds(line, 0);
	cmds = ft_calloc(sizeof(t_cmd *), nb_cmds + 1);
	if (!cmds)
		return (NULL);
	while (cmd_i < nb_cmds)
	{
		cmds[cmd_i] = init_cmd(line, env, &line_i);
		if (!cmds[cmd_i])
			return (free_cmds(cmds));
		cmd_i++;
	}
	return (cmds);
}

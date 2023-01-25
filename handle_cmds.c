/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:57 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 15:43:33 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//bizarre
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

void debug_log_cmds(t_cmd **cmds)
{
	for(int i = 0; cmds[i]; i++)
	{
		printf("cmd : %d\n", i);
		for (int j = 0; cmds[i]->args[j]; j++)
		{
			printf("\targs %d : %s\n", j, cmds[i]->args[j]);
		}
		if (cmds[i]->is_builtin)
			printf("cmd is builtin\n");
		if (cmds[i]->is_subshell)
			printf("cmd is subshell\n");
	}
}

int	handle_cmd(char *line, char ***env)
{
	t_cmd	**cmds;

	if (is_cmd_name_empty(line))
		return (0);
	if (!is_line_valid(line))
	{
		ft_printf_error("minishell: syntax error\n");
		set_les(258);
		return (1);
	}
	cmds = parse_line(line, *env);
	if (!cmds)
		return (-1);
	//debug_log_cmds(cmds);
	if (start_executing_cmds(cmds, env) == -1)
	{
		free_cmds(cmds);
		return (-1);
	}
	free_cmds(cmds);
	return (0);
}

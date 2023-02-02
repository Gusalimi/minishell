/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:38:43 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 14:58:04 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	free_all_return(int *quotes_pos, int *wildcards_pos, int rtn_value)
{
	free(quotes_pos);
	free(wildcards_pos);
	return (rtn_value);
}

//va falloir refactor ca
static int	check_wildcards(t_cmd *cmd, int *wildcards_pos,
	int wildcard_number, int wd_pos_i)
{
	int	i;
	int	j;

	i = -1;
	while (cmd->args[++i])
	{
		if (wildcards_pos[wd_pos_i] == 0)
			break ;
		j = -1;
		while (cmd->args[i][++j])
		{
			if (cmd->args[i][j] == '*')
			{
				if (wildcards_pos[wd_pos_i] == ++wildcard_number)
				{
					if (expand_wildcard(cmd, wildcards_pos,
							(int *[3]){&wildcard_number,
							&wd_pos_i, &i}, j) == -1)
						return (-1);
					break ;
				}
			}
		}
	}
	return (0);
}

int	finish_parsing(t_cmd *cmd, char **env)
{
	int		rtn_value;
	int		*quotes_pos;
	int		*wildcards_pos;

	rtn_value = init_cmd_iostream(cmd, env, 0);
	if (rtn_value == 2 || rtn_value == -1)
		return (rtn_value);
	cmd->is_subshell = is_cmd_subshell(cmd->name);
	if (cmd->is_subshell)
		return (0);
	quotes_pos = get_quotes_pos(cmd->args, env);
	wildcards_pos = get_wildcards_pos(cmd->args, env);
	if (!quotes_pos || !wildcards_pos || replace_env_variable(cmd, env) == -1)
		return (free_all_return(quotes_pos, wildcards_pos, -1));
	remove_quotes_pos(cmd->args, quotes_pos);
	check_wildcards(cmd, wildcards_pos, 0, 0);
	cmd->name = get_cmd_name(cmd->args, NULL);
	cmd->is_builtin = is_cmd_builtin(cmd->name);
	if (!cmd->is_builtin && !cmd->is_subshell)
	{
		cmd->path = get_cmd_path(cmd->name, env);
		if (!cmd->path)
			return (free_all_return(quotes_pos, wildcards_pos, -1));
	}
	return (free_all_return(quotes_pos, wildcards_pos, 0));
}

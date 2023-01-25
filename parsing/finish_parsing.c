/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:38:43 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 15:43:56 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	split_args(t_cmd *cmd)
{
	int		i;
	char	*joined_args;
	char	**tmp;

	i = 0;
	joined_args = ft_join(cmd->args, ' ');
	if (!joined_args)
		return (-1);
	tmp = get_cmd_args(joined_args, &i, FALSE);
	if (!tmp)
		return (-1);
	i = 0;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(joined_args);
	cmd->args = tmp;
	return (0);
}

static void	shift_left(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = s[i + 1];
		i++;
	}
}

static int	remove_quote(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd->args[i])
	{
		j = 0;
		while (cmd->args[i][j])
		{
			if ((cmd->args[i][j] == '\''
				|| cmd->args[i][j] == '"')
				&& is_ending_quote(cmd->args[i], j))
			{
				shift_left(&(cmd->args[i][j]));
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	finish_parsing(t_cmd *cmd, char **env)
{
	int	rtn_value;

	if (replace_env_variable(cmd, env) == -1)
		return (-1);
	rtn_value = init_cmd_iostream(cmd);
	if (rtn_value == 2 || rtn_value == -1)
		return (rtn_value);
	if (split_args(cmd) == -1)
		return (-1);
	if (remove_quote(cmd) == -1)
		return (-1);
	return (0);
}

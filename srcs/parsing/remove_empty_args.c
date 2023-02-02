/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_empty_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 13:38:59 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 16:03:13 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	has_empty_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (is_cmd_name_empty(cmd->args[i]))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static void	shift_left(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = s[i + 1];
		i++;
	}
}

void	remove_empty_args(t_cmd *cmd)
{
	int	i;

	if (!has_empty_args(cmd))
		return ;
	i = 0;
	while (cmd->args[i])
	{
		if (is_cmd_name_empty(cmd->args[i]))
		{
			free(cmd->args[i]);
			shift_left(&(cmd->args[i]));
			continue ;
		}
		i++;
	}
}

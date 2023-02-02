/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:27:38 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 14:38:19 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_cmd_args2(t_cmd *cmd, int *i, char **new_arg, int length[2])
{
	int	tmp;

	tmp = *i + 1;
	free(cmd->args[*i]);
	*i = *i + length[1] - 1;
	while (cmd->args[tmp])
		new_arg[length[0]++] = cmd->args[tmp++];
	free(cmd->args);
	cmd->args = new_arg;
	return (0);
}

static int	update_cmd_args(t_cmd *cmd, int *i, char **files)
{
	int		length;
	int		files_length;
	int		tmp;
	char	**new_arg;

	if (!files)
		return (0);
	length = 0;
	tmp = 0;
	files_length = 0;
	while (cmd->args[length])
		length++;
	while (files[files_length])
		files_length++;
	length += files_length;
	new_arg = malloc(sizeof(char *) * (length));
	if (!new_arg)
		return (-1);
	new_arg[length - 1] = 0;
	length = -1;
	while (++length < *i)
		new_arg[length] = cmd->args[length];
	while (tmp < files_length)
		new_arg[length++] = files[tmp++];
	return (update_cmd_args2(cmd, i, new_arg, (int [2]){length, files_length}));
}

//previous arg start AFTER the first parsed wildcards
static void	update_other_data(int *wdn_wdpi_i[3],
	int nb_parsed_wildcards, char *previous_arg)
{
	int	i;

	(*(wdn_wdpi_i[1])) += nb_parsed_wildcards;
	i = 0;
	while (previous_arg[i])
	{
		if (previous_arg[i] == '*')
			(*(wdn_wdpi_i[0]))++;
		i++;
	}
}

// wdn_wdpi_i[0] = &wildcard_number | wdn_wdpi_i[1] = &wd_pos_i
// wdn_wdpi_i[2] = &i
int	expand_wildcard(t_cmd *cmd, int *wildcards_pos,
	int *wdn_wdpi_i[3], int j)
{
	int		length;
	int		*arg_wd_pos;
	char	**new_args;
	int		error;

	length = 0;
	error = 0;
	arg_wd_pos = get_arg_wd_pos(cmd->args[*(wdn_wdpi_i[2])],
			wildcards_pos, *(wdn_wdpi_i[1]), j);
	if (!arg_wd_pos)
		return (-1);
	while (arg_wd_pos[length] != 0)
		length++;
	new_args = get_files_wildcard(cmd->args[*(wdn_wdpi_i[2])],
			&error, arg_wd_pos, length);
	free(arg_wd_pos);
	if (error == -1)
		return (-1);
	update_other_data(wdn_wdpi_i, length,
		&(cmd->args[*(wdn_wdpi_i[2])][j + 1]));
	if (update_cmd_args(cmd, wdn_wdpi_i[2], new_args) == -1)
		return (-1);
	free(new_args);
	return (0);
}

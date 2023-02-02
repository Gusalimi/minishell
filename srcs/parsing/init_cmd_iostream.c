/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_iostream.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:58:52 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 15:48:31 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_redir_static(t_cmd *cmd, int i)
{
	int	len;

	len = 0;
	while (cmd->args[len])
		len++;
	free(cmd->args[i]);
	free(cmd->args[i + 1]);
	while (i < len - 2)
	{
		cmd->args[i] = cmd->args[i + 2];
		i++;
	}
	cmd->args[len - 1] = 0;
	cmd->args[len - 2] = 0;
}

static int	get_filename(t_cmd *cmd, int len, int i, char **env)
{
	int		*quote_pos;
	char	*tmp;

	if (is_ambiguous(cmd, i, len, env) != 0)
	{
		ft_printf_error("minishell: %s: ambiguous redirect\n", cmd->args[i + 1]);
		return (2);
	}
	quote_pos = get_quotes_pos((char *[2]){cmd->args[i + 1], NULL}, env);
	if (!quote_pos)
		return (-1);
	tmp = replace_arg(cmd->args[i + 1], env);
	if (!tmp)
		return (-1);
	cmd->args[i + 1] = tmp;
	remove_quotes_pos((char *[2]){cmd->args[i + 1], NULL}, quote_pos);
	free(quote_pos);
	return (0);
}

static int	handle_right_redir(t_cmd *cmd, int len, int i, char **env)
{
	int	tmp;

	if (cmd->iostream.output_file_fd != 0)
		close(cmd->iostream.output_file_fd);
	tmp = get_filename(cmd, len, i, env);
	if (tmp != 0)
		return (tmp);
	if (access(cmd->args[i + 1], F_OK) == 0
		&& access(cmd->args[i + 1], W_OK) == -1)
		return (2);
	if (len == 1)
		cmd->iostream.output_file_fd = open(cmd->args[i + 1],
				O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		cmd->iostream.output_file_fd = open(cmd->args[i + 1],
				O_WRONLY | O_APPEND | O_CREAT, 0644);
	skip_redir_static(cmd, i);
	return (0);
}

static int	handle_left_redir(t_cmd *cmd, int len, int i, char **env)
{
	int	return_value;
	int	tmp;

	return_value = 0;
	if (cmd->iostream.input_file_fd != 0)
		close(cmd->iostream.input_file_fd);
	tmp = get_filename(cmd, len, i, env);
	if (tmp != 0)
		return (tmp);
	if (len == 1)
	{
		if (access(cmd->args[i + 1], R_OK) == -1)
		{
			ft_printf_error("minishell: ");
			perror(cmd->args[i + 1]);
			return (2);
		}
		cmd->iostream.input_file_fd = open(cmd->args[i + 1], O_RDONLY);
	}
	else
		return_value = handle_here_doc(cmd, i);
	skip_redir_static(cmd, i);
	return (return_value);
}

int	init_cmd_iostream(t_cmd *cmd, char **env, int i)
{
	int		len;
	int		return_value;

	return_value = 0;
	while (cmd->args[i])
	{
		len = ft_strlen(cmd->args[i]);
		if (cmd->args[i][0] == '>')
			return_value = handle_right_redir(cmd, len, i, env);
		else if (cmd->args[i][0] == '<')
			return_value = handle_left_redir(cmd, len, i, env);
		else
			i++;
		if (return_value == 2 || return_value == -1)
		{
			if (return_value == -1)
			{
				ft_printf_error("minishell: ");
				perror(cmd->args[i + 1]);
			}
			set_les(1);
			break ;
		}
	}
	return (return_value);
}

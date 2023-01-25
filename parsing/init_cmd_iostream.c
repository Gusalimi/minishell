/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_iostream.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:58:52 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 15:23:06 by gsaile           ###   ########.fr       */
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

static t_bool	has_dollar_out_quote(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && is_ending_quote(s, i))
		{
			skip_quotes(s, &i);
			continue ;
		}
		if (s[i] == '$')
			return (TRUE);
		i++;
	}
	printf("not ambiguous\n");
	return (FALSE);
}

static t_bool	is_ambiguous(t_cmd *cmd, int i, int len)
{
	//char	*var;
	//char	**split;

	if ((cmd->args[i][0] == '<' && len == 2) ||
	!has_dollar_out_quote(cmd->args[i + 1]))
		return (FALSE);
	ft_printf_error("ambiguous\n");
	i++;
	(void)len;
	//var = ft_getenv(cmd->args[i + 1]);
	//if ()
	return (TRUE);
}

static int	handle_right_redir(t_cmd *cmd, int len, int i)
{
	if (cmd->iostream.output_file_fd != 0)
		close(cmd->iostream.output_file_fd);
	if (is_ambiguous(cmd, i, len))
		return (2);
	cmd->iostream.has_output_file = TRUE;
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

static int	handle_left_redir(t_cmd *cmd, int len, int i)
{
	int	return_value;

	return_value = 0;
	if (cmd->iostream.input_file_fd != 0)
		close(cmd->iostream.input_file_fd);
	cmd->iostream.has_input_file = TRUE;
	if (len == 1)
	{
		if (access(cmd->args[i + 1], R_OK) == -1)
		{
			return (2);
		}
		cmd->iostream.input_file_fd = open(cmd->args[i + 1], O_RDONLY);
	}
	else
		return_value = handle_here_doc(cmd, i);
	skip_redir_static(cmd, i);
	return (return_value);
}

int	init_cmd_iostream(t_cmd *cmd)
{
	int		len;
	int		i;
	int		return_value;

	i = 0;
	return_value = 0;
	while (cmd->args[i])
	{
		len = ft_strlen(cmd->args[i]);
		if (cmd->args[i][0] == '>')
			return_value = handle_right_redir(cmd, len, i);
		else if (cmd->args[i][0] == '<')
			return_value = handle_left_redir(cmd, len, i);
		else
			i++;
		if (return_value == 2 || return_value == -1)
		{
			ft_printf_error("minishell: ");
			perror(cmd->args[i + 1]);
			set_les(1);
			break ;
		}
	}
	return (return_value);
}

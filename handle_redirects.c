/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 18:50:42 by lloison           #+#    #+#             */
/*   Updated: 2023/01/19 19:15:31 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redirects(t_iostream iostream)
{
	if (iostream.has_input_file)
	{
		if (dup2(iostream.input_file_fd, 0) == -1)
			return (-1);
		if (iostream.input_pipe[1] != 0)
			if (close (iostream.input_pipe[1]) == -1)
				return (-1);
	}
	else
	{
		if (iostream.input_pipe[0] != 0)
		{
			if (close (iostream.input_pipe[1]) == -1)
				return (-1);
			if (dup2(iostream.input_pipe[0], 0) == -1)
				return (-1);
		}
	}
	return (0);
}

static int	handle_output_redirects(t_iostream iostream)
{
	if (iostream.has_output_file)
	{
		if (dup2(iostream.output_file_fd, 1) == -1)
			return (-1);
		if (iostream.output_pipe[0] != 0)
			if (close (iostream.output_pipe[0]) == -1)
				return (-1);
	}
	else
	{
		if (iostream.output_pipe[1] != 0)
		{
			if (close (iostream.output_pipe[0]) == -1)
				return (-1);
			if (dup2(iostream.output_pipe[1], 1) == -1)
				return (-1);
		}
	}
	return (0);
}

int	handle_redirects(t_iostream iostream)
{
	if (handle_input_redirects(iostream) == -1)
		return (-1);
	if (handle_output_redirects(iostream) == -1)
		return (-1);
	return (0);
}

int	clear_cmd_fds(t_iostream *iostream)
{
	if (iostream->input_file_fd != 0)
	{
		if (close(iostream->input_file_fd) == -1)
			return (-1);
		iostream->input_file_fd = 0;
	}
	if (iostream->output_file_fd != 0)
	{
		if (close(iostream->output_file_fd) == -1)
			return (-1);
			iostream->output_file_fd = 0;
	}
	if (iostream->input_pipe[0] != 0)
	{
		if (close(iostream->input_pipe[0]) == -1)
			return (-1);
		iostream->input_pipe[0] = 0;
		if (close(iostream->input_pipe[1]) == -1)
			return (-1);
		iostream->input_pipe[1] = 0;
	}
	return (0);
}

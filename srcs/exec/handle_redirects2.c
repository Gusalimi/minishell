/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirects2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:56:06 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 15:39:02 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_input_redirects(t_iostream iostream)
{
	int	rtn_value;

	rtn_value = 0;
	if (iostream.input_file_fd != 0)
	{
		rtn_value = iostream.input_file_fd;
	}
	else
	{
		if (iostream.input_pipe[0] != 0)
		{
			rtn_value = iostream.input_pipe[0];
		}
	}
	return (rtn_value);
}

static int	get_output_redirects(t_iostream iostream)
{
	int	rtn_value;

	rtn_value = 1;
	if (iostream.output_file_fd != 0)
	{
		rtn_value = iostream.output_file_fd;
	}
	else
	{
		if (iostream.output_pipe[1] != 0)
		{
			rtn_value = iostream.output_pipe[1];
		}
	}
	return (rtn_value);
}

int	get_io_fds(t_iostream *iostream, int *input_fd, int *output_fd)
{
	int	rtn_value;

	if (!iostream)
	{
		*input_fd = 0;
		*output_fd = 1;
		return (0);
	}
	rtn_value = 0;
	rtn_value = get_input_redirects(*iostream);
	if (rtn_value == -1)
		return (-1);
	*input_fd = rtn_value;
	rtn_value = get_output_redirects(*iostream);
	if (rtn_value == -1)
		return (-1);
	*output_fd = rtn_value;
	return (0);
}

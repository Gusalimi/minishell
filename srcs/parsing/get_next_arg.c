/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 09:50:51 by lloison           #+#    #+#             */
/*   Updated: 2023/01/24 13:53:27 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	handle_quote(char *line, t_bool *in_q, int i)
{
	if (*in_q)
	{
		*in_q = FALSE;
		return (TRUE);
	}
	else
	{
		if (is_ending_quote(line, i))
		{
			*in_q = TRUE;
			return (TRUE);
		}
	}
	return (FALSE);
}

static int	get_arg_length(char *line, int i, int length, t_bool *in_par)
{
	t_bool	in_sq;
	t_bool	in_dq;

	in_sq = FALSE;
	in_dq = FALSE;
	if (*in_par)
		return (get_par_length(line, i));
	if (is_valid_symbols(line, i))
		return (get_valid_symbol_length(line, i));
	while (line[i] && (in_sq || in_dq || (!is_space(line[i])
				&& !is_valid_symbols(line, i))))
	{
		if (line[i] == '\'' && !in_dq)
			handle_quote(line, &in_sq, i);
		if (line[i] == '\"' && !in_sq)
			handle_quote(line, &in_dq, i);
		length++;
		i++;
	}
	return (length);
}

static void	copy_symbol(char *output, char *line, int *line_i, t_bool *in_par)
{
	output[0] = line[*line_i];
	*line_i = *line_i + 1;
	if (is_symbol_char(line[*line_i]))
	{
		output[1] = line[*line_i];
		*line_i = *line_i + 1;
	}
	if (output[0] == '(')
		*in_par = TRUE;
	else if (output[0] == ')')
		*in_par = FALSE;
}

static void	copy_arg(char *line, int *i, char *output, t_bool *in_par)
{
	int		output_i;
	t_bool	in_sq;
	t_bool	in_dq;

	output_i = 0;
	in_sq = FALSE;
	in_dq = FALSE;
	if (*in_par)
	{
		*in_par = FALSE;
		while (line[*i] && output[output_i])
			output[output_i++] = line[(*i)++];
		return ;
	}
	while (line[*i] && (in_sq || in_dq || (!is_space(line[*i])
				&& !is_valid_symbols(line, *i))))
	{
		if (line[*i] == '\'' && !in_dq)
			handle_quote(line, &in_sq, *i);
		else if (line[*i] == '\"' && !in_sq)
			handle_quote(line, &in_dq, *i);
		if (line[*i])
			output[output_i++] = line[(*i)++];
	}
}

char	*get_next_arg(char *line, int *line_i, t_bool *in_par)
{
	int		length;
	char	*output;

	while (is_space(line[*line_i]))
		*line_i = *line_i + 1;
	length = get_arg_length(line, *line_i, 0, in_par);
	output = malloc(sizeof(char) * (length + 1));
	if (!output)
		return (NULL);
	ft_memset(output, 1, sizeof(char) * (length + 1));
	output[length] = 0;
	if (!(*in_par) && is_valid_symbols(line, *line_i))
	{
		copy_symbol(output, line, line_i, in_par);
		return (output);
	}
	copy_arg(line, line_i, output, in_par);
	return (output);
}

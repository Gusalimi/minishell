/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:33:13 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 16:08:25 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_skips(char *line, int *start, int *output, t_bool *read_text)
{
	while (line[*start] == '\'' || line[*start] == '"')
	{
		skip_quotes(line, start);
		if (read_text)
			*read_text = TRUE;
	}
	if (line[*start] && is_redir(line[*start], line[*start + 1]))
	{
		if (output)
			*output = *output + 1 + *read_text;
		if (read_text)
			*read_text = FALSE;
		skip_redir(line, start);
	}
	else if (line[*start] == '(')
	{
		if (output)
			*output = *output + 2;
		skip_parenthesis(line, start);
		if (read_text)
			*read_text = FALSE;
	}
}

static void	get_start_and_end(char *line, int i, int *start, int *end)
{
	*start = 0;
	*end = 0;
	if (i != 0)
	{
		if (is_breakpoint(line[i], line[i + 1]))
			skip_breakpoint(line, &i);
		while (line[i] && is_space(line[i]))
			i++;
		*start = i;
	}
	while (line[i] && line[i + 1] && !is_breakpoint(line[i], line[i + 1]))
	{
		handle_skips(line, &i, NULL, NULL);
		if (line[i])
			i++;
	}
	if (line[i] && !is_breakpoint(line[i], line[i + 1]))
	{
		while (is_space(line[i]))
			i--;
		*end = i;
	}
	else
		*end = i - 1;
}

static int	get_nb_args(char *line, int start, int end)
{
	t_bool	read_text;
	int		output;

	read_text = FALSE;
	output = 0;
	while (start <= end)
	{
		handle_skips(line, &start, &output, &read_text);
		if (is_space(line[start]))
		{
			if (read_text)
			{
				read_text = FALSE;
				output++;
			}
			while (is_space(line[start]))
				start++;
			continue ;
		}
		else
			read_text = TRUE;
		start++;
	}
	return (output + read_text);
}

static void	*free_output(char **output)
{
	int	i;

	i = 0;
	while (output[i])
	{
		free(output[i]);
		i++;
	}
	free(output);
	return (NULL);
}

char	**get_cmd_args(char *line, int *line_i, t_bool in_par)
{
	int		start;
	int		end;
	int		nb_args;
	char	**output;
	int		i;

	start = 0;
	end = 0;
	get_start_and_end(line, *line_i, &start, &end);
	nb_args = get_nb_args(line, start, end);
	output = ft_calloc(sizeof(char *), (nb_args + 1));
	if (!output)
		return (NULL);
	output[nb_args] = 0;
	i = 0;
	while (i < nb_args)
	{
		output[i] = get_next_arg(line, line_i, &in_par);
		while (is_space(line[*line_i]))
			*line_i = *line_i + 1;
		if (!output[i])
			return (free_output(output));
		i++;
	}
	return (output);
}

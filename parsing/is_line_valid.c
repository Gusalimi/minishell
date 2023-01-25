/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_line_valid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:06:48 by lloison           #+#    #+#             */
/*   Updated: 2023/01/24 12:38:31 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	check_invalid_symbols(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			skip_quotes(line, &i);
			continue ;
		}
		else if (is_symbol_char(line[i]))
		{
			if (is_valid_symbols(line, i))
			{
				skip_valid_symbols(line, &i);
				continue ;
			}
			else
				return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

static t_bool	check_consec_breakpoint(char *line, int i)
{
	int	flag;

	flag = 1;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			flag = 0;
			skip_quotes(line, &i);
			continue ;
		}
		if (is_breakpoint(line[i], line[i + 1]))
		{
			if (flag == 1)
				return (FALSE);
			flag = 1;
			skip_breakpoint(line, &i);
			continue ;
		}
		else if (flag == 1 && !is_space(line[i]))
			flag = 0;
		i++;
	}
	return (flag == 0);
}

static t_bool	check_consec_redir(char *line)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			flag = 0;
			skip_quotes(line, &i);
			continue ;
		}
		else if (is_redir(line[i], line [i + 1]))
		{
			if (flag == 1)
				return (FALSE);
			flag = 1;
			skip_redir(line, &i);
		}
		else if (flag == 1 && !is_space(line[i]))
			flag = 0;
		i++;
	}
	return (flag == 0);
}

static t_bool	check_redir_has_file(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			skip_quotes(line, &i);
			continue ;
		}
		if (is_redir(line[i], line[i + 1]))
		{
			skip_redir(line, &i);
			while (is_space(line[i]))
				i++;
			if (!line[i] || is_symbol_char(line[i]))
				return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

t_bool	is_line_valid(char *line)
{
	int	nb_braces;

	if (ft_strlen(line) == 0 || is_cmd_name_empty(line))
		return (TRUE);
	if (check_invalid_symbols(line) == FALSE)
		return (FALSE);
	if (check_consec_breakpoint(line, 0) == FALSE)
		return (FALSE);
	if (check_consec_redir(line) == FALSE)
		return (FALSE);
	if (check_redir_has_file(line) == FALSE)
		return (FALSE);
	if (check_nb_braces(line, &nb_braces) == FALSE)
		return (FALSE);
	if (check_braces_positions(line, nb_braces) == FALSE)
		return (FALSE);
	if (check_for_no_empty_braces(line) == FALSE)
		return (FALSE);
	return (TRUE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 11:06:33 by lloison           #+#    #+#             */
/*   Updated: 2023/01/23 18:36:01 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_ending_quote(char *line, int i)
{
	char	quote;

	quote = line[i];
	while (line[i])
	{
		i++;
		if (line[i] == quote)
			return (TRUE);
	}
	return (FALSE);
}

t_bool	is_redir(char c1, char c2)
{
	if ((c1 == '<' && c2 == '<')
		|| (c1 == '<' && c2 != '>')
		|| (c1 == '>' && c2 == '>')
		|| (c1 == '>' && c2 != '<'))
		return (TRUE);
	return (FALSE);
}

void	skip_redir(char *line, int *main_i)
{
	if (line[*main_i] == '>')
	{
		*main_i = *main_i + 1;
		if (line[*main_i] == '>')
			*main_i = *main_i + 1;
	}
	else if (line[*main_i] == '<')
	{
		*main_i = *main_i + 1;
		if (line[*main_i] == '<')
			*main_i = *main_i + 1;
	}
}

t_bool	is_symbol_char(char c)
{
	if (c == '|' || c == '&'
		|| c == '>' || c == '<')
		return (TRUE);
	return (FALSE);
}

t_bool	is_valid_symbols(char *line, int i)
{
	if (!line[i])
		return (FALSE);
	if (!line[i + 1] && (line[i] == '|'
			|| line[i] == '>' || line[i] == '<'
			|| line[i] == ')'))
		return (TRUE);
	else if (!line[i + 1])
		return (FALSE);
	if ((line[i] == '|' && !is_symbol_char(line[i + 1]))
		|| (line[i] == '|' && line[i + 1] == '|'
			&& !is_symbol_char(line[i + 2]))
		|| (line[i] == '&' && line[i + 1] == '&'
			&& !is_symbol_char(line[i + 2]))
		|| (line[i] == '>' && !is_symbol_char(line[i + 1]))
		|| (line[i] == '>' && line[i + 1] == '>'
			&& !is_symbol_char(line[i + 2]))
		|| (line[i] == '<' && !is_symbol_char(line[i + 1]))
		|| (line[i] == '<' && line[i + 1] == '<'
			&& !is_symbol_char(line[i + 2])))
		return (TRUE);
	if (line[i] == '(' || line[i] == ')')
		return (TRUE);
	return (FALSE);
}

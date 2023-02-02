/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:47:30 by lloison           #+#    #+#             */
/*   Updated: 2023/02/01 12:08:08 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_breakpoint(char c1, char c2)
{
	if ((c1 == '|' && c2 == '|')
		|| (c1 == '&' && c2 == '&')
		|| (c1 == '|' && c2 != '&'))
		return (TRUE);
	return (FALSE);
}

void	skip_breakpoint(char *line, int *main_i)
{
	if (line[*main_i] == '&')
		*main_i = *main_i + 2;
	else if (line[*main_i] == '|' && line[*main_i + 1] == '|')
		*main_i = *main_i + 2;
	else
		*main_i = *main_i + 1;
}

t_bool	is_space(char c)
{
	if (c != '\t' && c != '\n' && c != '\v' && c != '\f'
		&& c != '\r' && c != ' ')
		return (FALSE);
	return (TRUE);
}

t_bool	skip_quotes(char *line, int *main_i)
{
	int		i;
	char	quote;

	i = *main_i + 1;
	quote = line[i - 1];
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		*main_i = i;
	else
	{
		*main_i = i + 1;
	}
	return (TRUE);
}

t_bool	skip_dollar(char *line, int *main_i)
{
	*main_i = *main_i + 1;
	if (ft_isdigit(line[*main_i]))
	{
		*main_i = *main_i + 1;
		return (TRUE);
	}
	if (line[*main_i] == '?')
	{
		*main_i = *main_i + 1;
		return (TRUE);
	}
	while (line[*main_i] && (ft_isalnum(line[*main_i]) || line[*main_i] == '_'))
		*main_i = *main_i + 1;
	return (TRUE);
}

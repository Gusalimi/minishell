/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:01:18 by lloison           #+#    #+#             */
/*   Updated: 2023/01/24 12:17:58 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_redir_and_filename(char *line, int *i)
{
	skip_redir(line, i);
	while (line[*i] && is_space(line[*i]))
		*i = *i + 1;
	while (line[*i] && !is_space(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			if (is_ending_quote(line, *i))
			{
				skip_quotes(line, i);
				continue ;
			}
		}
		if (is_valid_symbols(line, *i))
			break ;
		*i = *i + 1;
	}
}

void	skip_one_arg(char *line, int *i)
{
	while (line[*i] && !is_space(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			if (is_ending_quote(line, *i))
			{
				skip_quotes(line, i);
				continue ;
			}
		}
		if (is_valid_symbols(line, *i))
			break ;
		*i = *i + 1;
	}
}

void	skip_parenthesis(char *line, int *i)
{
	int	count;

	count = 0;
	if (line[*i] == '(')
	{
		count++;
		*i = *i + 1;
	}
	while (count != 0)
	{
		if (line[*i] == '(')
			count++;
		else if (line[*i] == ')')
			count--;
		if (count > 0)
			*i = *i + 1;
	}
}

static void	skip_br_par(char *line, int *i, int *flag)
{
	if (is_breakpoint(line[*i], line[*i + 1]))
	{
		skip_breakpoint(line, i);
		*flag = 1;
	}
	else if (line[*i] == '(')
		skip_parenthesis(line, i);
}

int	get_nb_cmds(char *line, int i)
{
	int	output;
	int	flag;

	output = 1;
	flag = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
			skip_quotes(line, &i);
		else if (is_breakpoint(line[i], line[i + 1]) || line[i] == '(')
		{
			skip_br_par(line, &i, &flag);
			continue ;
		}
		if (flag == 1 && !is_space(line[i]))
		{
			flag = 0;
			output++;
		}
		if (line[i])
			i++;
	}
	return (output);
}

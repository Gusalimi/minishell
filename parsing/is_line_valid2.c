/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_line_valid2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 14:08:52 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 15:32:07 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	check_nb_braces(char *s, int *total_count)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	*total_count = 0;
	while (s[++i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			skip_quotes(s, &i);
			continue ;
		}
		if (s[i] == '(')
		{
			*total_count = *total_count + 1;
			count++;
		}
		else if (s[i] == ')')
			count--;
		if (count < 0)
			return (FALSE);
	}
	return (count == 0);
}

static int	count_valid_left_braces(char *s)
{
	int		count;
	int		i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && (is_space(s[i]) || s[i] == '('))
		{
			if (s[i] == '(')
				count++;
			i++;
		}
		while (s[i] && !is_breakpoint(s[i], s[i + 1]))
			i++;
		if (s[i] && is_breakpoint(s[i], s[i + 1]))
		{
			skip_breakpoint(s, &i);
			continue ;
		}
		if (s[i])
			i++;
	}
	return (count);
}

static int	tmp_count_in_redir(char *s, int *i)
{
	int	count;

	count = 0;
	while (s[*i] && (is_space(s[*i]) || is_redir(s[*i], s[*i + 1])
			|| s[*i] == ')' || s[*i] == '(')
		&& !is_breakpoint(s[*i], s[*i + 1]))
	{
		if (is_redir(s[*i], s[*i + 1]))
		{
			skip_redir_and_filename(s, i);
			continue ;
		}
		if (s[*i] == ')')
			count++;
		*i = *i + 1;
	}
	return (count);
}

static int	count_valid_right_braces(char *s)
{
	int	tmp_count;
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && !is_breakpoint(s[i], s[i + 1]))
		{
			tmp_count = tmp_count_in_redir(s, &i);
			if (s[i] && !is_valid_symbols(s, i))
			{
				skip_one_arg(s, &i);
				tmp_count = 0;
			}
		}
		count += tmp_count;
		if (s[i])
			skip_breakpoint(s, &i);
	}
	return (count);
}

t_bool	check_braces_positions(char *s, int nb_braces)
{
	if (nb_braces != count_valid_left_braces(s)
		|| nb_braces != count_valid_right_braces(s))
		return (FALSE);
	return (TRUE);
}

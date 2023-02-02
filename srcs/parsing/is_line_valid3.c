/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_line_valid3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 12:32:28 by lloison           #+#    #+#             */
/*   Updated: 2023/01/24 12:39:29 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	is_empty_brace(char *s, int i)
{
	int		count;
	t_bool	read_text;

	count = 1;
	i++;
	read_text = FALSE;
	while (count != 0)
	{
		if (s[i] == '(')
			count++;
		else if (s[i] == ')')
			count--;
		if (count > 0)
		{
			if (!is_space(s[i]) && !is_valid_symbols(s, i))
				read_text = TRUE;
			i++;
		}
	}
	return (!read_text);
}

t_bool	check_for_no_empty_braces(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '(')
			if (is_empty_brace(s, i) == TRUE)
				return (FALSE);
		i++;
	}
	return (TRUE);
}

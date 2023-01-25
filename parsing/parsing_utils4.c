/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 11:17:04 by lloison           #+#    #+#             */
/*   Updated: 2023/01/23 12:35:46 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	is_capital(char c)
{
	return (c >= 'A' && c <= 'Z');
}

static t_bool	is_min(char c)
{
	return (c >= 'a' && c <= 'z');
}

static t_bool	is_different_no_case(char c1, char c2)
{
	if (is_capital(c1) && is_min(c2))
	{
		if (c1 - 'A' + 'a' != c2)
			return (TRUE);
	}
	else if (is_min(c1) && is_capital(c2))
	{
		if (c1 != c2 - 'A' + 'a')
			return (TRUE);
	}
	else if (c1 != c2)
		return (TRUE);
	return (FALSE);
}

int	compare_text_no_case(char *s1, char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2)
		return (1);
	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] && s2[i] && i < n - 1)
	{
		if (is_different_no_case(s1[i], s2[i]))
			return (1);
		i++;
	}
	if (is_different_no_case(s1[i], s2[i]))
		return (1);
	return (0);
}

t_bool	is_dir(char *filename)
{
	struct stat	path;

	stat(filename, &path);
	if (S_ISDIR(path.st_mode))
		return (TRUE);
	return (FALSE);
}

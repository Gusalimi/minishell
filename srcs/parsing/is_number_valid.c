/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_number_valid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 15:20:36 by gsaile            #+#    #+#             */
/*   Updated: 2023/02/02 14:53:53 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	has_alpha(const char *s)
{
	while (is_space(*s))
		s++;
	if (*s == '+' || *s == '-')
		s++;
	while (*s)
	{
		if ((*s < '0' || *s > '9') && !is_space(*s))
			return (TRUE);
		s++;
	}
	return (FALSE);
}

static t_bool	is_number_valid2(char *tmp,
	unsigned long long int result, int sign)
{
	while (is_space(*tmp))
		tmp++;
	while (ft_isdigit(*tmp))
	{
		result = result * 10 + *tmp - '0';
		tmp++;
		if ((sign == 1 && result > 9223372036854775807L)
			|| (sign == -1 && result > 9223372036854775808ULL))
			return (FALSE);
	}
	return (TRUE);
}

t_bool	is_number_valid(const char *str)
{
	unsigned long long int	result;
	int						sign;
	char					*tmp;

	result = 0;
	sign = 1;
	tmp = (char *)str;
	if (!str || has_alpha(str))
		return (FALSE);
	if (*tmp == '+' || *tmp == '-')
		if (*(tmp++) == '-')
			sign = -1;
	while (*tmp == '0')
		tmp++;
	return (is_number_valid2(tmp, result, sign));
}

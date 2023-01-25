/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 13:26:09 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 15:13:13 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_par_length(char *s, int i)
{
	int	output;
	int	count;

	count = 1;
	output = 0;
	while (count != 0)
	{
		if (s[i] == '(')
			count++;
		else if (s[i] == ')')
			count--;
		if (count > 0)
		{
			output++;
			i++;
		}
	}
	return (output);
}

t_bool	is_cmd_subshell(char *s)
{
	if (!s)
		return (FALSE);
	if (s[0] == '(')
		return (TRUE);
	return (FALSE);
}

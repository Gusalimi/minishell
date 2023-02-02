/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 13:29:30 by lloison           #+#    #+#             */
/*   Updated: 2023/02/01 13:29:49 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	nb_wildcard_in_var(char *arg, char **env)
{
	char	*var;
	int		i;
	int		output;

	i = 0;
	output = 0;
	var = ft_getenv(arg, env);
	if (!var)
		return (0);
	while (var[i])
	{
		if (var[i] == '*')
			output++;
		i++;
	}
	return (output);
}

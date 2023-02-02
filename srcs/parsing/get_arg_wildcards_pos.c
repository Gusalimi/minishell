/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg_wildcards_pos.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 17:32:59 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 13:30:29 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nb_wildcard_to_expand(char *arg, int *wildcards_pos, int i)
{
	int	output;
	int	wd_pos_i;
	int	wd_number;

	wd_pos_i = 1;
	output = 1;
	wd_number = 1;
	i++;
	while (arg[i])
	{
		if (arg[i] == '*')
		{
			if (wildcards_pos[wd_pos_i] - wildcards_pos[0] == wd_number)
			{
				output++;
				wd_pos_i++;
			}
			wd_number++;
		}
		i++;
	}
	return (output);
}

static int	get_start_shift(char *arg, int first_wd_pos)
{
	int	i;
	int	start_shift;

	i = 0;
	start_shift = 0;
	while (arg[i])
	{
		if (arg[i] == '*')
		{
			if (i == first_wd_pos)
				return (start_shift);
			start_shift++;
		}
		i++;
	}
	return (start_shift);
}

static void	fill_new_wildcard_pos(int *nwd_pos, int *wildcard_pos,
	int start_shift, int nb_wildcard_to_parse)
{
	int	i;

	nwd_pos[0] = start_shift + 1;
	i = 1;
	while (i < nb_wildcard_to_parse)
	{
		nwd_pos[i] = start_shift + wildcard_pos[i] - wildcard_pos[0] + 1;
		i++;
	}
}

int	*get_arg_wd_pos(char *arg, int *wildcards_pos,
	int wd_pos_i, int first_wd_pos)
{
	int	nb_wd_to_expand;
	int	*new_wildcard_pos;
	int	start_shift;

	nb_wd_to_expand = get_nb_wildcard_to_expand(arg,
			&wildcards_pos[wd_pos_i], first_wd_pos);
	new_wildcard_pos = ft_calloc(sizeof(int), (nb_wd_to_expand + 1));
	if (!new_wildcard_pos)
		return (NULL);
	new_wildcard_pos[nb_wd_to_expand] = 0;
	start_shift = get_start_shift(arg, first_wd_pos);
	fill_new_wildcard_pos(new_wildcard_pos,
		&wildcards_pos[wd_pos_i], start_shift, nb_wd_to_expand);
	return (new_wildcard_pos);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_cmd_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:48:02 by lloison           #+#    #+#             */
/*   Updated: 2023/01/20 11:56:02 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	handle_ics(int caller, t_bool value)
{
	static t_bool	ics = FALSE;

	if (caller == 0)
		return (ics);
	else if (caller == 1)
		ics = value;
	return (FALSE);
}

t_bool	get_ics(void)
{
	return (handle_ics(0, FALSE));
}

void	set_ics(t_bool value)
{
	handle_ics(1, value);
}

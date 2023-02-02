/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_subshell_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 11:54:03 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/26 11:56:28 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	handle_ifs(int caller, t_bool value)
{
	static t_bool	ifs = FALSE;

	if (caller == 0)
		return (ifs);
	else if (caller == 1)
		ifs = value;
	return (FALSE);
}

t_bool	get_ifs(void)
{
	return (handle_ifs(0, FALSE));
}

void	set_ifs(t_bool value)
{
	handle_ifs(1, value);
}

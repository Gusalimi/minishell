/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:17:42 by gsaile            #+#    #+#             */
/*   Updated: 2023/02/02 16:03:53 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	bi_echo2(char **argv, int count, int i);
static void	bi_echo3(char **argv, int *i, t_bool *nl);

int	bi_echo(char **argv, char **envp)
{
	int		i;
	int		count;
	t_bool	nl;

	(void)envp;
	nl = TRUE;
	i = 1;
	count = 0;
	while (argv[count])
		count++;
	if (count > 1)
		bi_echo3(argv, &i, &nl);
	if (bi_echo2(argv, count, i) == -1)
		return (-1);
	if (nl)
		if (printf("\n") < 0)
			return (-1);
	return (0);
}

int	bi_echo2(char **argv, int count, int i)
{
	while (i < count)
	{
		if (printf("%s", argv[i++]) < 0)
			return (-1);
		if (i < count)
			if (printf(" ") < 0)
				return (-1);
	}
	return (0);
}

static void	bi_echo3(char **argv, int *i, t_bool *nl)
{
	int	j;

	while (argv[*i] && argv[*i][0] == '-')
	{
		j = 1;
		while (argv[*i][j] == 'n')
			j++;
		if (argv[*i][j] != 0)
			break ;
		*nl = FALSE;
		(*i)++;
	}
}

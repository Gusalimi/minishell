/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var_in_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 17:46:40 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 16:10:53 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_buf(char *buf)
{
	int	i;

	i = 0;
	while (i < 1000)
	{
		buf[i] = 0;
		i++;
	}
}

//aae[0] is args | aae[1] is env
//i[0] is arg_i | i[1] is i | i[2] is buf_i
static t_bool	loop(char **aae[2], int *i[3], char *buf, t_bool *in_quote)
{
	if (aae[0][(*(i[0]))][(*(i[1]))] == '\''
		&& is_ending_quote(aae[0][(*(i[0]))], (*(i[1]))))
	{
		buf[(*(i[2]))++] = aae[0][(*(i[0]))][(*(i[1]))++];
		while (aae[0][(*(i[0]))][(*(i[1]))] != '\'')
			buf[(*(i[2]))++] = aae[0][(*(i[0]))][(*(i[1]))++];
		buf[(*(i[2]))++] = aae[0][(*(i[0]))][(*(i[1]))++];
		return (TRUE);
	}
	if (*in_quote && aae[0][(*(i[0]))][(*(i[1]))] == '"')
		*in_quote = FALSE;
	else if (aae[0][(*(i[0]))][(*(i[1]))] == '"'
		&& is_ending_quote(aae[0][(*(i[0]))], (*(i[1]))))
		*in_quote = TRUE;
	if (*in_quote && aae[0][(*(i[0]))][(*(i[1]))] == '$')
	{
		copy_var_to_buf(buf, i[2], &(aae[0][*(i[0])][(*(i[1]))]), aae[1]);
		skip_dollar(aae[0][(*(i[0]))], (i[1]));
		return (TRUE);
	}
	return (FALSE);
}

int	replace_var_in_quotes(char **args, int arg_i, char **env)
{
	char	buf[1000];
	int		i;
	int		buf_i;
	t_bool	in_quote;
	char	*tmp;

	i = 0;
	buf_i = 0;
	in_quote = FALSE;
	clear_buf(buf);
	while (args[arg_i][i])
	{
		if (loop((char **[2]){args, env},
			(int *[3]){&arg_i, &i, &buf_i}, buf, &in_quote))
			continue ;
		buf[buf_i++] = args[arg_i][i++];
	}
	tmp = ft_strdup(buf);
	if (!tmp)
		return (-1);
	free(args[arg_i]);
	args[arg_i] = tmp;
	return (0);
}

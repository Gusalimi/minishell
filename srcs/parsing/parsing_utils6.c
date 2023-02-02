/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 13:26:09 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 17:21:24 by lloison          ###   ########.fr       */
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

void	copy_var_to_buf(char *buf, int *buf_i, char *arg, char **env)
{
	int		i;
	char	*var;

	var = ft_getenv(arg, env);
	if (!var)
		return ;
	i = 0;
	while (var[i])
	{
		buf[*buf_i] = var[i];
		*buf_i = *buf_i + 1;
		i++;
	}
}

void	split_copy_var_to_buf(char *buf, int *buf_i, char *arg, char **env)
{
	int		i;
	char	*var;
	t_bool	skipping_spaces;

	var = ft_getenv(arg, env);
	if (!var)
		return ;
	i = 0;
	skipping_spaces = FALSE;
	while (var[i])
	{
		if (is_space(var[i]) && *buf_i != 0 && buf[*buf_i - 1])
		{
			*buf_i = *buf_i + 1;
		}
		if (!is_space(var[i]))
		{
			buf[*buf_i] = var[i];
			*buf_i = *buf_i + 1;
		}
		i++;
	}
}

t_bool	arg_has_wildcards(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if ((arg[i] == '\'' || arg[i] == '"') && is_ending_quote(arg, i))
		{
			skip_quotes(arg, &i);
			continue ;
		}
		if (arg[i] == '*')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

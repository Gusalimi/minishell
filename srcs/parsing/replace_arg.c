/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 12:31:42 by lloison           #+#    #+#             */
/*   Updated: 2023/02/01 13:25:52 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_length(char *arg, char **env)
{
	int		i;
	int		length;
	t_bool	parse_dollar;

	length = 0;
	i = 0;
	parse_dollar = TRUE;
	while (arg[i])
	{
		if (arg[i] == '$' && parse_dollar)
		{
			length += ft_strlen(ft_getenv(&(arg[i]), env));
			skip_dollar(arg, &i);
			continue ;
		}
		if (parse_dollar && arg[i] == '\'' && is_ending_quote(arg, i))
			parse_dollar = FALSE;
		else if (!parse_dollar && arg[i] == '\'')
			parse_dollar = TRUE;
		i++;
		length++;
	}
	return (length);
}

static void	copy_var(char *arg, char *output, int *output_i, char **env)
{
	char	*var;
	int		i;

	var = ft_getenv(arg, env);
	if (!var)
		return ;
	i = 0;
	while (var[i])
	{
		output[*output_i] = var[i];
		*output_i = *output_i + 1;
		i++;
	}
}

//a_o[0] is arg | a_o[1] is output | i_oi[0] is i | i_oi[1] is output_i
static t_bool	handle_quotes(char *a_o[2], int *ioi[2], t_bool *p, char **env)
{
	int	tmp;

	if (a_o[0][*(ioi[0])] == '"' && is_ending_quote(a_o[0], *(ioi[0])))
	{
		a_o[1][(*(ioi[1]))++] = a_o[0][(*(ioi[0]))++];
		while (a_o[0][*(ioi[0])] != '"')
		{
			if (a_o[0][*(ioi[0])] == '$' && *p)
			{
				tmp = *(ioi[0]);
				copy_var(&(a_o[0][*(ioi[0])]), a_o[1], (ioi[1]), env);
				skip_dollar(a_o[0], (ioi[0]));
				continue ;
			}
			a_o[1][(*(ioi[1]))++] = a_o[0][(*(ioi[0]))++];
		}
		if (a_o[0][*(ioi[0])] != '"')
			a_o[1][(*(ioi[1]))++] = a_o[0][(*(ioi[0]))++];
	}
	else if (*p && a_o[0][*(ioi[0])] == '\''
		&& is_ending_quote(a_o[0], *(ioi[0])))
		*p = FALSE;
	else if (!(*p) && a_o[0][*(ioi[0])] == '\'')
		*p = TRUE;
	return (FALSE);
}

static void	copy_arg(char *arg, char *output, char **env)
{
	int		i;
	int		output_i;
	t_bool	parse_dollar;
	int		tmp;

	output_i = 0;
	i = 0;
	parse_dollar = TRUE;
	while (arg[i])
	{
		if (arg[i] == '$' && parse_dollar)
		{
			tmp = i;
			copy_var(&(arg[i]), output, &output_i, env);
			skip_dollar(arg, &i);
			continue ;
		}
		if (arg[i] == '\'' || arg[i] == '"')
		{
			if (handle_quotes((char *[2]){arg, output},
				(int *[2]){&i, &output_i}, &parse_dollar, env))
				continue ;
		}
		output[output_i++] = arg[i++];
	}
}

char	*replace_arg(char *arg, char **env)
{
	char	*output;
	int		length;

	length = get_length(arg, env);
	output = ft_calloc(sizeof(char), (length + 1));
	if (!output)
		return (NULL);
	output[length] = 0;
	copy_arg(arg, output, env);
	free(arg);
	if (is_cmd_name_empty(output))
		output[0] = 0;
	return (output);
}

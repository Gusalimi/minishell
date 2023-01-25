/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_variable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:56:56 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 14:37:15 by lloison          ###   ########.fr       */
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
		if (parse_dollar && arg[i] == '\'' && is_ending_quote(arg, i))
			parse_dollar = FALSE;
		else if (!parse_dollar && arg[i] == '\'')
			parse_dollar = TRUE;
		output[output_i++] = arg[i++];
	}
}

static char	*replace_arg(char *arg, char **env)
{
	char	*output;
	int		length;

	length = get_length(arg, env);
	output = malloc(sizeof(char) * (length + 1));
	if (!output)
		return (NULL);
	output[length] = 0;
	copy_arg(arg, output, env);
	return (output);
}

int	replace_env_variable(t_cmd *cmd, char **env)
{
	int	i;
	int	j;

	i = 0;
	while (cmd->args[i])
	{
		j = 0;
		while (cmd->args[i][j])
		{
			if (cmd->args[i][j] == '\'')
			{
				skip_quotes(cmd->args[i], &j);
				continue ;
			}
			if (cmd->args[i][j] == '$')
			{
				cmd->args[i] = replace_arg(cmd->args[i], env);
				if (!cmd->args[i])
					return (-1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

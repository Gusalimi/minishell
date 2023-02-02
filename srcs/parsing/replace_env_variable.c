/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_variable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:56:56 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 15:03:13 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	env_var_has_space(char *s)
{
	int	i;

	if (!s)
		return (FALSE);
	i = 0;
	if (is_space(s[0]) || is_space(s[ft_strlen(s) - 1]))
		return (TRUE);
	while (s[i] && is_space(s[i]))
		i++;
	while (s[i] && !is_space(s[i]))
		i++;
	while (s[i] && is_space(s[i]))
		i++;
	if (s[i])
		return (TRUE);
	return (FALSE);
}

static t_bool	multiple_args_needed(char *arg, char **env)
{
	int		i;
	t_bool	parse_dollar;

	i = 0;
	parse_dollar = TRUE;
	while (arg[i])
	{
		if (arg[i] == '$' && parse_dollar)
		{
			if (env_var_has_space(ft_getenv(&(arg[i]), env)))
				return (TRUE);
			skip_dollar(arg, &i);
			continue ;
		}
		if (parse_dollar && arg[i] == '\'' && is_ending_quote(arg, i))
			parse_dollar = FALSE;
		else if (!parse_dollar && arg[i] == '\'')
			parse_dollar = TRUE;
		i++;
	}
	return (FALSE);
}

static int	modify_args(t_cmd *cmd, int *i, char **env)
{
	int	length;
	int	new_length;

	if (multiple_args_needed(cmd->args[*i], env))
	{
		length = 0;
		while (cmd->args[length])
			length++;
		cmd->args = get_updated_args(cmd->args, *i, env);
		if (!cmd->args)
			return (-1);
		new_length = 0;
		while (cmd->args[new_length])
			new_length++;
		*i = *i + (new_length - length);
	}
	else
	{
		cmd->args[*i] = replace_arg(cmd->args[*i], env);
		if (!cmd->args[*i])
			return (-1);
	}
	return (0);
}

static int	handle_loop(t_cmd *cmd, int *i, int *j, char **env)
{
	while (cmd->args[*i][*j])
	{
		if (cmd->args[*i][*j] == '\'' && is_ending_quote(cmd->args[*i], *j)
			&& skip_quotes(cmd->args[*i], j))
			continue ;
		else if (cmd->args[*i][*j] == '"' && is_ending_quote(cmd->args[*i], *j))
		{
			while (cmd->args[*i][++(*j)] != '"')
			{
				if (cmd->args[*i][*j] == '$')
				{
					if (modify_args(cmd, i, env) == -1)
						return (-1);
					break ;
				}
			}
		}
		if (cmd->args[*i][*j] == '$')
			return (modify_args(cmd, i, env));
		(*j)++;
	}
	return (0);
}

int	replace_env_variable(t_cmd *cmd, char **env)
{
	int	i;
	int	j;

	i = -1;
	while (cmd->args[++i])
	{
		j = 0;
		handle_loop(cmd, &i, &j, env);
	}
	return (0);
}

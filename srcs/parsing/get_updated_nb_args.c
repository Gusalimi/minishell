/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_updated_nb_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:26:39 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 17:59:00 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	has_text_after_dollar(char *arg)
{
	int	i;

	i = 0;
	skip_dollar(arg, &i);
	while (is_space(arg[i]))
		i++;
	while (arg[i] == '\'' || arg[i] == '"')
	{
		if (arg[i + 1] != arg[i])
			return (TRUE);
		i += 2;
	}
	if (arg[i])
		return (TRUE);
	return (FALSE);
}

static int	calculate_nb_new_arg(char *arg, char **env, t_bool read_text)
{
	int		output;
	char	*var;
	char	**tmp;
	int		i;

	output = 0;
	var = ft_getenv(arg, env);
	if (!var || is_cmd_name_empty(var))
		return (0);
	if (is_space(var[0]) && read_text)
		output++;
	if (is_space(var[ft_strlen(var) - 1]) && has_text_after_dollar(arg))
			output++;
	tmp = ft_split_is_space(var);
	if (!tmp)
		return (-1);
	i = 0;
	while (tmp[i])
		i++;
	output += i - 1;
	free(tmp);
	return (output);
}

static int	handle_dollar(char *arg, char **env, t_bool read_text, int *i)
{
	int	tmp;

	tmp = calculate_nb_new_arg(arg, env, read_text);
	if (tmp == -1)
		return (-1);
	skip_dollar(arg, i);
	return (tmp);
}

static int	get_nb_args_to_add(char *arg, char **env, int i, int output)
{
	t_bool	read_text;

	read_text = FALSE;
	while (arg[i])
	{
		if ((arg[i] == '\'' || arg[i] == '"') && is_ending_quote(arg, i))
		{
			if (arg[i + 1] != arg[i])
				read_text = TRUE;
			skip_quotes(arg, &i);
			continue ;
		}
		if (arg[i] == '$')
		{
			output += handle_dollar(&(arg[i]), env, read_text, &i);
			continue ;
		}
		if (!read_text && !is_space(arg[i]))
			read_text = TRUE;
		i++;
	}
	return (output);
}

int	get_updated_nb_args(char **args, int arg_i, char **env)
{
	int	current_nb_args;
	int	nb_args_to_add;

	current_nb_args = 0;
	while (args[current_nb_args])
		current_nb_args++;
	nb_args_to_add = get_nb_args_to_add(args[arg_i], env, 0, 1);
	if (nb_args_to_add == -1)
		return (-1);
	return (current_nb_args + nb_args_to_add - 1);
}

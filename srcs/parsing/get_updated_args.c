/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_updated_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 12:32:57 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 16:17:19 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*free_chars_and_return(char **args, char **output)
{
	int	i;

	if (args)
	{
		i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
	}
	if (output)
	{
		i = 0;
		while (output[i])
			free(output[i++]);
		free(output);
	}
	return (NULL);
}

static int	extract_args_from_buf(char *buf, char **output, int *output_i)
{
	int	buf_i;

	buf_i = 0;
	while (buf[buf_i])
	{
		output[*output_i] = ft_strdup(&(buf[buf_i]));
		if (!output[*output_i])
			return (-1);
		*output_i = *output_i + 1;
		buf_i += ft_strlen(&(buf[buf_i])) + 1;
	}
	return (0);
}

static int	create_new_args(char **output, char *arg, int *output_i, char **env)
{
	char	buf[1000];
	int		buf_i;
	int		i;

	i = 0;
	buf_i = -1;
	while (++(buf_i) < 1000)
		buf[buf_i] = 0;
	while (arg[i])
	{
		fill_buf(buf, arg, &i, env);
		if (extract_args_from_buf(buf, output, output_i) == -1)
			return (-1);
		buf_i = -1;
		while (++buf_i < 1000)
			buf[buf_i] = 0;
	}
	return (0);
}

static int	fill_output(char **output, char **args, int arg_i, char **env)
{
	int	i;
	int	output_i;

	i = 0;
	output_i = 0;
	while (i < arg_i)
	{
		output[output_i] = args[i];
		i++;
		output_i++;
	}
	if (replace_var_in_quotes(args, arg_i, env) == -1)
		return (-1);
	if (create_new_args(output, args[arg_i], &output_i, env) == -1)
		return (-1);
	i = arg_i + 1;
	while (args[i])
	{
		output[output_i] = args[i];
		i++;
		output_i++;
	}
	return (0);
}

char	**get_updated_args(char **args, int arg_i, char **env)
{
	char	**output;
	int		nb_args;

	nb_args = get_updated_nb_args(args, arg_i, env);
	if (nb_args == -1)
		return (free_chars_and_return(args, NULL));
	output = ft_calloc(sizeof(char *), (nb_args + 1));
	if (!output)
		return (free_chars_and_return(args, NULL));
	output[nb_args] = 0;
	if (fill_output(output, args, arg_i, env) == -1)
		return (free_chars_and_return(args, output));
	return (output);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wildcards_pos.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 11:38:20 by lloison           #+#    #+#             */
/*   Updated: 2023/02/01 13:29:55 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nb_wildcards_to_remove(char **args, char **env)
{
	int	i;
	int	j;
	int	output;

	i = 0;
	output = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if ((args[i][j] == '\'' || args[i][j] == '"')
				&& is_ending_quote(args[i], j)
				&& skip_quotes(args[i], &j))
				continue ;
			if (args[i][j] == '$')
				output += nb_wildcard_in_var(&args[i][j], env);
			if (args[i][j] == '*')
				output++;
			j++;
		}
		i++;
	}
	return (output);
}

static t_bool	update_wd_pos(int *output, char *var,
	int *output_i, int *wd_number)
{
	int		i;

	if (!var)
		return (TRUE);
	i = 0;
	while (var[i])
	{
		if (var[i] == '*')
			output[(*output_i)++] = ++(*wd_number);
		i++;
	}
	return (TRUE);
}

static void	handle_quote(char *arg, int *wdn_j[2], char **env)
{
	char	quote;

	quote = arg[*(wdn_j[1])];
	(*(wdn_j[1]))++;
	while (arg[*(wdn_j[1])] != quote)
	{
		if (quote == '"' && arg[*(wdn_j[1])] == '$')
		{
			(*(wdn_j[0])) += nb_wildcard_in_var(&(arg[*(wdn_j[1])]), env);
			skip_dollar(arg, (wdn_j[1]));
			continue ;
		}
		if (arg[*(wdn_j[1])] == '*')
			(*(wdn_j[0]))++;
		(*(wdn_j[1]))++;
	}
	(*(wdn_j[1]))++;
}

static void	fill_wildcards_pos(int *output, char **args, char **env, int ioi[2])
{
	int		j;
	int		wd_number;

	wd_number = 0;
	while (args[++(ioi[0])])
	{
		j = 0;
		while (args[ioi[0]][j])
		{
			if (args[ioi[0]][j] == '$'
				&& update_wd_pos(output,
					ft_getenv(&(args[ioi[0]][j]), env), &(ioi[1]), &wd_number)
				&& skip_dollar(args[ioi[0]], &j))
				continue ;
			if ((args[ioi[0]][j] == '\'' || args[ioi[0]][j] == '"')
				&& is_ending_quote(args[ioi[0]], j))
			{
				handle_quote(args[ioi[0]], (int *[2]){&wd_number, &j}, env);
				continue ;
			}
			if (args[ioi[0]][j] == '*')
				output[(ioi[1])++] = ++wd_number;
			j++;
		}
	}
}

int	*get_wildcards_pos(char **args, char **env)
{
	int	*output;
	int	nb_wildcards_to_remove;

	nb_wildcards_to_remove = get_nb_wildcards_to_remove(args, env);
	output = malloc(sizeof(int) * (nb_wildcards_to_remove + 1));
	if (!output)
		return (NULL);
	output[nb_wildcards_to_remove] = 0;
	fill_wildcards_pos(output, args, env, (int [2]){-1, 0});
	return (output);
}

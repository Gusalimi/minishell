/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_quotes_pos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:07:14 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 17:47:20 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nb_quotes_to_remove(char **args)
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
				&& is_ending_quote(args[i], j))
			{
				output += 2;
				skip_quotes(args[i], &j);
				continue ;
			}
			j++;
		}
		i++;
	}
	return (output);
}

static void	update_quote_number(int *quote_number, char *arg, char **env)
{
	char	*var;
	int		i;

	var = ft_getenv(arg, env);
	if (!var)
		return ;
	i = 0;
	while (var[i])
	{
		if (var[i] == '"' || var[i] == '\'')
			(*quote_number)++;
		i++;
	}
}

static int	handle_quote(int *output, char *arg, int *qn_j[2], char **env)
{
	char	quote;
	int		output_i;

	quote = arg[*(qn_j[1])];
	output_i = 0;
	output[output_i] = *(qn_j[0]) + 1;
	output_i++;
	(*(qn_j[0]))++;
	(*(qn_j[1]))++;
	while (arg[*(qn_j[1])] != quote)
	{
		if (quote == '"' && arg[*(qn_j[1])] == '$')
		{
			update_quote_number((qn_j[0]), &(arg[*(qn_j[1])]), env);
			skip_dollar(arg, (qn_j[1]));
			continue ;
		}
		if (arg[*(qn_j[1])] == '\'' || arg[*(qn_j[1])] == '"')
			(*(qn_j[0]))++;
		(*(qn_j[1]))++;
	}
	output[output_i++] = *(qn_j[0]) + 1;
	(*(qn_j[0]))++;
	(*(qn_j[1]))++;
	return (output_i);
}

static void	fill_quotes_pos(int *output, char **args, char **env, int ioi[2])
{
	int		j;
	int		quote_number;

	quote_number = 0;
	while (args[++(ioi[0])])
	{
		j = 0;
		while (args[ioi[0]][j])
		{
			if (args[ioi[0]][j] == '$')
			{
				update_quote_number(&quote_number, &(args[ioi[0]][j]), env);
				skip_dollar(args[ioi[0]], &j);
				continue ;
			}
			if ((args[ioi[0]][j] == '\'' || args[ioi[0]][j] == '"')
				&& is_ending_quote(args[ioi[0]], j))
			{
				ioi[1] += handle_quote(&(output[ioi[1]]),
						args[ioi[0]], (int *[2]){&quote_number, &j}, env);
				continue ;
			}
			j++;
		}
	}
}

int	*get_quotes_pos(char **args, char **env)
{
	int		*output;
	int		nb_quotes_to_remove;

	nb_quotes_to_remove = get_nb_quotes_to_remove(args);
	output = malloc(sizeof(int) * (nb_quotes_to_remove + 1));
	if (!output)
		return (NULL);
	output[nb_quotes_to_remove] = 0;
	fill_quotes_pos(output, args, env, (int [2]){-1, 0});
	return (output);
}

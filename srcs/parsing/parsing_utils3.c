/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 17:52:03 by lloison           #+#    #+#             */
/*   Updated: 2023/01/26 15:28:58 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_valid_symbols(char *line, int *i)
{
	if (line[*i] == '|')
	{
		*i = *i + 1;
		if (line[*i] == '|')
			*i = *i + 1;
	}
	else if (line[*i] == '&' && line[*i + 1] == '&')
		*i = *i + 2;
	else if (line[*i] == '>')
	{
		*i = *i + 1;
		if (line[*i] == '>')
			*i = *i + 1;
	}
	else if (line[*i] == '<')
	{
		*i = *i + 1;
		if (line[*i] == '<')
			*i = *i + 1;
	}
}

int	get_valid_symbol_length(char *line, int i)
{
	if ((line[i] == '|' && !is_symbol_char(line[i + 1]))
		|| (line[i] == '<' && !is_symbol_char(line[i + 1]))
		|| (line[i] == '>' && !is_symbol_char(line[i + 1]))
		|| line [i] == '(' || line[i] == ')')
		return (1);
	return (2);
}

t_bool	is_cmd_builtin(char *cmd)
{
	if (!cmd)
		return (TRUE);
	if (cmd[0] == 0)
		return (FALSE);
	if (compare_text_no_case("echo", cmd, 5) == 0
		|| compare_text_no_case("cd", cmd, 3) == 0
		|| compare_text_no_case("pwd", cmd, 4) == 0
		|| compare_text_no_case("export", cmd, 7) == 0
		|| compare_text_no_case("unset", cmd, 6) == 0
		|| compare_text_no_case("env", cmd, 4) == 0
		|| compare_text_no_case("exit", cmd, 5) == 0)
		return (TRUE);
	return (FALSE);
}

char	*get_cmd_name(char **args, char *line)
{
	int	i;

	if (line && line[0] == 0)
		return (NULL);
	i = 0;
	while (args[i] && args[i + 1] && is_redir(args[i][0], args[i][1]))
		i += 2;
	return (args[i]);
}

t_bool	is_cmd_name_empty(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (TRUE);
	while (s[i])
	{
		if (!is_space(s[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_updated_args_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:35:52 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 14:30:06 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	should_stop_here(char *arg, char **env, t_bool read_text)
{
	char	*var;

	if (is_space(arg[0]))
		return (TRUE);
	if (arg[0] != '$')
		return (FALSE);
	var = ft_getenv(arg, env);
	if (!var)
		return (FALSE);
	if (is_space(var[0]) && read_text)
		return (TRUE);
	return (FALSE);
}

static void	handle_quote(char *buf, char *arg, int *arg_i, int *buf_i)
{
	char	quote;

	quote = arg[*arg_i];
	buf[(*buf_i)++] = arg[(*arg_i)++];
	while (arg[*arg_i] != quote)
		buf[(*buf_i)++] = arg[(*arg_i)++];
	buf[(*buf_i)++] = arg[(*arg_i)++];
}

//s[0] is arg | s[1] is buf | i[0] is arg_i, | i[1] is i
static t_bool	handle_dollar(char *s[2],
	int *i[2], char **env, t_bool read_text)
{
	if (should_stop_here(&(s[0][*(i[0])]), env, read_text))
		return (TRUE);
	else
		split_copy_var_to_buf(s[1], i[1], &(s[0][*(i[0])]), env);
	skip_dollar(s[0], i[0]);
	if (should_stop_here(&(s[0][*(i[0])]), env, read_text)
		|| is_space(s[1][ft_strlen(s[1]) - 1]))
		return (TRUE);
	return (FALSE);
}

void	fill_buf(char *buf, char *arg, int *arg_i, char **env)
{
	int		i;
	t_bool	read_text;

	i = 0;
	read_text = FALSE;
	while (arg[*arg_i])
	{
		if ((arg[*arg_i] == '\'' || arg[*arg_i] == '"')
			&& is_ending_quote(arg, *arg_i))
		{
			handle_quote(buf, arg, arg_i, &i);
			continue ;
		}
		if (arg[*arg_i] == '$')
		{
			if (handle_dollar((char *[2]){arg, buf},
				(int *[2]){arg_i, &i}, env, read_text))
				break ;
			continue ;
		}
		if (!read_text && !is_space(arg[*arg_i]))
			read_text = TRUE;
		else
			buf[i++] = arg[(*arg_i)++];
	}
}

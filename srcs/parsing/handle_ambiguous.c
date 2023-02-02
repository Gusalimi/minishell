/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ambiguous.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 18:04:16 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/31 13:22:26 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	has_dollar_out_quote(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && is_ending_quote(s, i))
		{
			skip_quotes(s, &i);
			continue ;
		}
		if (s[i] == '$')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static char	*remove_in_quotes2(char *s, int len)
{
	char	*new;
	int		i;
	int		j;

	new = malloc((len + 1) * sizeof(char));
	if (!new)
		return (NULL);
	new[len] = 0;
	i = 0;
	j = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '"') && is_ending_quote(s, i))
			skip_quotes(s, &i);
		else
			new[j++] = s[i++];
	}
	free(s);
	return (new);
}

static char	*remove_in_quotes(char *s)
{
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '"') && is_ending_quote(s, i))
			skip_quotes(s, &i);
		else
		{
			len++;
			i++;
		}
	}
	return (remove_in_quotes2(s, len));
}

int	is_ambiguous(t_cmd *cmd, int i, int len, char **env)
{
	char	*no_quotes;
	char	**tmp;

	if ((cmd->args[i][0] == '<' && len == 2) ||
	!has_dollar_out_quote(cmd->args[i + 1]))
		return (0);
	no_quotes = ft_strdup(cmd->args[i + 1]);
	if (!no_quotes)
		return (-1);
	no_quotes = remove_in_quotes(no_quotes);
	if (!no_quotes)
		return (-1);
	no_quotes = replace_arg(no_quotes, env);
	if (!no_quotes)
		return (-1);
	tmp = ft_split_is_space(no_quotes);
	if (!tmp)
		return (-1);
	if (tmp[1])
		return (1);
	free_all(tmp);
	free(no_quotes);
	return (0);
}

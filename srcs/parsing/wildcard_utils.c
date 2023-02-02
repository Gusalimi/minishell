/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 16:37:02 by gsaile            #+#    #+#             */
/*   Updated: 2023/02/02 14:36:42 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	list_remove_if(t_list **lst, t_bool (*test)(), char **param)
{
	t_list	*remove;
	t_list	*current;

	current = *lst;
	while (current && current->next)
	{
		if (test(current->next->content, param))
		{
			remove = current->next;
			current->next = current->next->next;
			free(remove->content);
			free(remove);
		}
		else
			current = current->next;
	}
	current = *lst;
	if (current && test(current->content, param))
	{
		*lst = current->next;
		free(current->content);
		free(current);
	}
}

t_bool	hidden(char *file, char **origin)
{
	(void)origin;
	return (*file == '.');
}

t_bool	begin(char *file, char **origin)
{
	char	*begin;

	begin = *origin;
	if (!begin)
		return (FALSE);
	if (ft_strncmp(file, begin, ft_strlen(begin)) == 0)
		return (FALSE);
	return (TRUE);
}

t_bool	end(char *file, char **origin)
{
	char	*ref;
	char	*end;

	if (!file || !origin || !(*origin))
		return (TRUE);
	ref = *origin;
	if (ft_strlen(ref) > ft_strlen(file))
		return (TRUE);
	end = &(file[ft_strlen(file) - ft_strlen(ref)]);
	if (!end)
		return (FALSE);
	if (ft_strcmp(ref, end) == 0)
		return (FALSE);
	return (TRUE);
}

t_bool	between(char *file, char **no_stars)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (no_stars[i] && file[j])
	{
		if (ft_strncmp(file + j, no_stars[i], ft_strlen(no_stars[i])) == 0)
		{
			j += ft_strlen(no_stars[i]);
			i++;
			continue ;
		}
		else
			j++;
	}
	if (no_stars[i] && !file[j])
		return (TRUE);
	return (FALSE);
}

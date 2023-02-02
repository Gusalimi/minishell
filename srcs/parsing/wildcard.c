/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 14:24:15 by gsaile            #+#    #+#             */
/*   Updated: 2023/02/02 15:20:30 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*get_files(struct dirent *file,
	int *error, DIR *dir, t_list *files)
{
	t_list	*tmp_l;
	char	*tmp_s;

	while (file)
	{
		tmp_s = ft_strdup(file->d_name);
		if (!tmp_s)
		{
			*error = -1;
			ft_lstclear(&files, free);
			return (NULL);
		}
		tmp_l = ft_lstnew(tmp_s);
		if (!tmp_l)
		{
			*error = -1;
			free(tmp_s);
			ft_lstclear(&files, free);
			return (NULL);
		}
		ft_lstadd_back(&files, tmp_l);
		file = readdir(dir);
	}
	return (files);
}

static int	to_tab2(t_list *files, int *error, int *size, char ***tab)
{
	t_list	*tmp_l;

	tmp_l = files;
	while (tmp_l)
	{
		(*size)++;
		tmp_l = tmp_l->next;
	}
	*tab = malloc((*size + 1) * sizeof(char *));
	if (!(*tab))
	{
		ft_lstclear(&files, free);
		*error = -1;
		return (-1);
	}
	(*tab)[*size] = 0;
	return (0);
}

static char	**to_tab(t_list *files, int *error)
{
	t_list	*tmp_l;
	char	**tab;
	int		i;
	int		size;

	i = 0;
	size = 0;
	if (!files)
		return (NULL);
	if (to_tab2(files, error, &size, &tab) == -1)
		return (NULL);
	tmp_l = files;
	while (tmp_l)
	{
		tab[i++] = tmp_l->content;
		tmp_l = tmp_l->next;
	}
	while (files)
	{
		tmp_l = files->next;
		free(files);
		files = tmp_l;
	}
	sort_env(&tab);
	return (tab);
}

//nbwd_i_j_n[0] is nb_wd | nbwd_i_j_n[1] is i
//nbwd_i_j_n[2] is j | nbwd_i_j_n[3] is n
static int	filter_list(t_list **files, char *s, int *wd_pos, int nbwd_i_j_n[4])
{
	char	**tab;

	tab = split_wd(s, wd_pos, nbwd_i_j_n[0]);
	if (!tab)
	{
		ft_lstclear(files, free);
		return (-1);
	}
	if (*s != '.')
		list_remove_if(files, hidden, NULL);
	if (*s != '*' || wd_pos[0] != 1)
		list_remove_if(files, begin, tab);
	while (s[++(nbwd_i_j_n[1])])
		if (s[nbwd_i_j_n[1]] == '*')
			(nbwd_i_j_n[3])++;
	while (tab[nbwd_i_j_n[2]])
		(nbwd_i_j_n[2])++;
	if (s[nbwd_i_j_n[1] - 1] != '*'
		|| wd_pos[nbwd_i_j_n[0] - 1] != nbwd_i_j_n[3])
		list_remove_if(files, end, tab + nbwd_i_j_n[2] - 1);
	list_remove_if(files, between, tab);
	free_all(tab);
	return (0);
}

char	**get_files_wildcard(char *s, int *error, int *wd_pos, int nb_wd)
{
	DIR				*dir;
	struct dirent	*file;
	t_list			*files;

	dir = opendir(".");
	if (!dir)
	{
		*error = -1;
		return (NULL);
	}
	file = readdir(dir);
	files = get_files(file, error, dir, NULL);
	closedir(dir);
	if (!files)
		return (NULL);
	if (filter_list(&files, s, wd_pos, (int [4]){nb_wd, -1, 0, 0}) == -1)
	{
		*error = -1;
		return (NULL);
	}
	return (to_tab(files, error));
}

// int main()
// {
// 	int	error;
// 	char **tab = get_files_wildcard("*", &error, (int [1]){1}, 1);
// 	for (int i = 0; tab[i]; i++)
// 		printf("tab[i] = %s\n", tab[i]);
// }

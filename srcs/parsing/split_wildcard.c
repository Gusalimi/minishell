/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:56:08 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 15:31:33 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_nb_words(char const *s, int *wd_pos, int nb_wd, int i)
{
	int	j;
	int	n;
	int	nb_words;
	int	counting;

	counting = 0;
	nb_words = 0;
	n = 1;
	j = 0;
	while (s[i])
	{
		if (s[i] == '*' && counting == 1 && n++ == wd_pos[j])
		{
			nb_words++;
			counting = 0;
			if (++j >= nb_wd)
				j = 0;
		}
		else if (s[i] == '*' && n != wd_pos[j])
			n++;
		else if (s[i] != '*' && counting == 0)
			counting = 1;
		i++;
	}
	return (nb_words + counting);
}

static char	*ft_strncpy(char *dest, char const *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

static char	*get_next_word(char const *s, size_t *ind, int *wd_pos, int nb_wd)
{
	char	*output;
	size_t	length;
	size_t	start_index;
	int		n;
	int		j;

	length = 0;
	j = 0;
	n = 0;
	while (s[(*ind)++] == '*' && ++n == wd_pos[j])
		if (++j >= nb_wd)
			j = 0;
	start_index = *ind;
	while (s[*ind] && (s[*ind] != '*' || (s[*ind] == '*' && n != wd_pos[j])))
	{
		if (s[(*ind)++] == '*')
			n++;
		length++;
	}
	output = (char *) malloc(sizeof(char) * (length + 1));
	if (output == 0)
		return (0);
	ft_strncpy(output, &s[start_index], length);
	output[length] = '\0';
	return (output);
}

static void	free_if_error(char **output, size_t index)
{
	size_t	i;

	i = 0;
	while (i < index)
	{
		free(output[i]);
		i++;
	}
	free(output);
}

char	**split_wd(char const *s, int *wd_pos, int nb_wd)
{
	size_t	i;
	size_t	nb_words;
	size_t	s_index;
	char	**output;

	i = 0;
	if (s == 0)
		return (0);
	nb_words = count_nb_words(s, wd_pos, nb_wd, 0);
	s_index = 0;
	output = (char **) ft_calloc(sizeof(char *), (nb_words + 1));
	if (output == 0)
		return (0);
	while (i < nb_words)
	{
		output[i] = get_next_word(s, &s_index, wd_pos, nb_wd);
		if (output[i] == 0)
		{
			free_if_error(output, i);
			return (0);
		}
		i++;
	}
	return (output);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_is_space.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:27:16 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/25 11:31:47 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_nb_words(char const *s)
{
	size_t	i;
	size_t	nb_words;
	int		counting;

	i = 0;
	counting = 0;
	nb_words = 0;
	while (s[i])
	{
		if (is_space(s[i]) && counting == 1)
		{
			nb_words++;
			counting = 0;
		}
		else if (!is_space(s[i]) && counting == 0)
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

static char	*get_next_word(char const *s, size_t *index)
{
	char	*output;
	size_t	length;
	size_t	start_index;

	length = 0;
	while (is_space(s[*index]))
		*index = *index + 1;
	start_index = *index;
	while (s[*index] && !is_space(s[*index]))
	{
		length++;
		*index = *index + 1;
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

char	**ft_split_is_space(char const *s)
{
	size_t	i;
	size_t	nb_words;
	size_t	s_index;
	char	**output;

	i = 0;
	if (s == 0)
		return (0);
	nb_words = count_nb_words(s);
	s_index = 0;
	output = (char **) malloc(sizeof(char *) * (nb_words + 1));
	if (output == 0)
		return (0);
	while (i < nb_words)
	{
		output[i] = get_next_word(s, &s_index);
		if (output[i] == 0)
		{
			free_if_error(output, i);
			return (0);
		}
		i++;
	}
	output[i] = 0;
	return (output);
}
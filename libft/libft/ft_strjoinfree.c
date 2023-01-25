/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinfree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 10:55:42 by lloison           #+#    #+#             */
/*   Updated: 2022/11/05 14:12:10 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*join_str(char *s1, char *s2,
	size_t size_s1, size_t size_s2)
{
	char	*output;
	size_t	i;
	size_t	j;

	output = (char *) malloc((size_s1 + size_s2 + 1) * sizeof(char));
	if (output == 0)
		return (0);
	i = -1;
	while (++i < size_s1)
		output[i] = s1[i];
	j = -1;
	while (++j < size_s2)
	{
		output[i] = s2[j];
		i++;
	}
	output[i] = '\0';
	return (output);
}

char	*ft_strjoinfree(char *s1, char *s2)
{
	size_t	size_s1;
	size_t	size_s2;
	char	*output;

	if (s1 == 0)
		return (s2);
	if (s2 == 0)
		return (s1);
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	output = join_str(s1, s2, size_s1, size_s2);
	free(s1);
	free(s2);
	return (output);
}

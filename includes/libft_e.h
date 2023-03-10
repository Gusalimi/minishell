/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_e.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 14:23:25 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 16:14:49 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_E_H
# define LIBFT_E_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include "../srcs/libft/libft/libft.h"
# include "../srcs/libft/sstring.h"
# define MAX_INT 2147483647
# define MIN_INT -2147483648

int			ft_printf(const char *s, ...);
int			ft_printf_error(const char *s, ...);
char		*generate_fstring(const char *s, ...);
char		*get_next_line(int fd);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iostream.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 15:36:48 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 16:12:01 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOSTREAM_H
# define IOSTREAM_H

# include "libft_e.h"

typedef struct s_iostream
{
	t_bool	has_input_file;
	t_bool	has_output_file;
	int		input_file_fd;
	int		output_file_fd;
	int		input_pipe[2];
	int		output_pipe[2];
}	t_iostream;

#endif
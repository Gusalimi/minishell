/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iostream.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 15:36:48 by lloison           #+#    #+#             */
/*   Updated: 2023/01/19 12:44:57 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOSTREAM_H
# define IOSTREAM_H

# include "libft/libft_e.h"

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
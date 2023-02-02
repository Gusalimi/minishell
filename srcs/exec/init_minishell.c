/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 18:49:19 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 15:07:29 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	overwrite_ctrl_c(int s)
{
	(void)s;
	printf("\033[1A\033[%zuC  %c%c\n", ft_strlen(rl_line_buffer) + 2, 8, 8);
	exit(1);
}

static void	handle_ctrl_c(int s)
{
	(void)s;
	rl_redisplay();
	write(0, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	if (!get_ics())
		rl_redisplay();
}

static void	handle_ctrl_bs(int s)
{
	(void)s;
}

int	init_minishell(char **env, char ***envp, int *fd, char **line)
{
	rl_catch_signals = 0;
	*envp = create_env(env);
	if (!(*envp))
	{
		ft_printf_error("Failed to initialize environment\n");
		return (-1);
	}
	if (shlvl(envp) == -1)
		return (free_all(*envp));
	if (bi_unset((char *[3]){"unset", "OLDPWD", NULL}, envp) == -1)
		return (free_all(*envp));
	if (bi_export((char *[3]){"export", "OLDPWD", NULL}, envp, NULL) == -1)
		return (free_all(*envp));
	if (set_pwd(envp) == -1)
		return (free_all(*envp));
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, handle_ctrl_bs);
	*fd = create_history();
	if (*fd == -1)
	{
		free_all(*envp);
		return (-1);
	}
	*line = readline("minishell> ");
	return (0);
}

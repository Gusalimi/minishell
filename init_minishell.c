/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 18:49:19 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 12:59:09 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	overwrite_ctrl_c(int s)
{
	(void)s;
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

static int	shlvl(char ***envp)
{
	char	*oshlvl_s;
	char	*nshlvl_s;
	char	*var;
	int		oshlvl_i;

	oshlvl_s = ft_getenv("$SHLVL", *envp);
	if (!oshlvl_s || !is_number_valid(oshlvl_s) || oshlvl_s[0] == '-')
	{
		printf("here\n");
		if (!is_number_valid(oshlvl_s) && !has_alpha(oshlvl_s)
			&& oshlvl_s[0] != '-')
			ft_printf_error("minishell: warning: shell level (%s)%s", oshlvl_s,
				"too high, resetting to 1\n");
		bi_export((char *[3]){"export", "SHLVL=1", NULL}, envp, NULL);
		return (0);
	}
	oshlvl_i = ft_atol(oshlvl_s);
	nshlvl_s = ft_itoa(oshlvl_i + 1);
	if (!nshlvl_s)
		return (-1);
	var = ft_strjoin("SHLVL=", nshlvl_s);
	free(nshlvl_s);
	bi_export((char *[3]){"export", var, NULL}, envp, NULL);
	free(var);
	return (0);
}

int	init_minishell(char **env, char ***envp, int *fd, char **line)
{
	rl_catch_signals = 0;
	*envp = create_env(env);
	shlvl(envp);
	bi_unset((char *[3]){"unset", "OLDPWD", NULL}, envp);
	bi_export((char *[3]){"export", "OLDPWD", NULL}, envp, NULL);
	if (!*envp)
	{
		ft_printf_error("Failed to initialize environment\n");
		return (-1);
	}
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

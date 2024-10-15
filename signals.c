/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:33:38 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/15 11:27:46 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signal.h"

void	handle_norm_sig(int sig)
{
	if (sig == SIGINT)
	{
		ft_putendl_fd("", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		g_global = 130;
	}
}

void	handle_hd_sig(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		rl_on_new_line();
		write(2, "\n", 1);
		g_global = 130;
		exit (130);
	}
}

void	sig_def(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	signal_handle(int status)
{
	if (WIFEXITED(status))
		g_global = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_global = 128 + WTERMSIG(status);
}

void	wif_herdoc(int status)
{
	if (WIFEXITED(status))
	{
		if (status == 256)
			g_global = 1;
	}
	else if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT))
		g_global = 1;
	signal_handle(status);
}

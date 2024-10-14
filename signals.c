/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:33:38 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/14 18:28:16 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signal.h"

//signal is the function to intiate signals
//SIGINT cntrl-c
//SIGQUIT cntrl-\

void	handle_norm_sig(int sig) //for cntrl-c
{
	if (sig == SIGINT)
	{
		ft_putendl_fd("", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		g_global = 130;
		// should be globar assign ex_stat to 130;
	}
}

void	handle_hd_sig(int sig) //for cntrl-c in heredock
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		rl_on_new_line();
		write(2, "\n", 1);
		g_global = 130;
		//rl_redisplay();
		exit (130);
	}
}

void	sig_def(void) // to set signals to defolt
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	signal_handle(int status) // to update ex_value after performing simple command
{
	if (WIFEXITED(status)) //returns true if child exited normally with exit;
		g_global = WEXITSTATUS(status); //return exit status of child;
	else if (WIFSIGNALED(status)) //returns nn-zero if child terminated bc it recieve signal that hasnt been handled
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit: \n", 2);
			g_global = 131;
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			ft_putstr_fd("\n", 2);
			g_global = 130;
		}
	}
}

void	wif_herdoc(int status) // to update exx_value after performing heredock (I need heredock child for this one!
{
	if (WIFEXITED(status))
	{
		if (status == 256)
		{
			write(2, "enter 1\n", 8);
			g_global = 1;
		}
	}
	else if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT))
	{
		write(2, "enter 2\n", 8);
		g_global = 1;
	}
	signal_handle(status);
}

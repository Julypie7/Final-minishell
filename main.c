/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 12:56:35 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/15 13:55:44 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "lexer.h"
#include "struct.h"
#include "parsing.h"
#include "execution.h"
#include "expansion.h"

int	g_global;

int	init_struct(t_info *info, char **env)
{
	info->rl = NULL;
	info->ex_stat = 0;
	info->prev_ex_stat = 0;
	info->envp = env_to_list(env);
	if (!info->envp)
		return (1);
	info->copy = NULL;
	info->tokens = NULL;
	return (0);
}

void	init_sign(t_info *info)
{
	g_global = 0;
	signal(SIGINT, handle_norm_sig);
	signal(SIGQUIT, SIG_IGN);
	info->prev_ex_stat = info->ex_stat;
	info->ex_stat = g_global;
	info->rl = readline("our minishell: ");
	if (info->rl)
	{
		if (info->rl[0])
			add_history(info->rl);
	}
}

void	second_part_exec(t_info *info, t_cmd *cmds)
{
	cmds = tkn_to_cmd(info->tokens);
	if (!cmds)
		exit_tkn_to_cmd(info);
	free_lexlst(info->tokens);
	info->tokens = NULL;
	info->ex_stat = executor(cmds, info);
	printf("exit status: %d\n", info->ex_stat);
	free_cmds(cmds);
	rl_clear_history();
	free(info->rl);
	if (info->ex_stat == -1)
		exit (-1);
}

int	start_reading(t_info *info)
{
	t_cmd	*cmds;

	while (1)
	{
		init_sign(info);
		if (!info->rl)
			return (1);
		if (!valid_line(info))
		{
			free(info->rl);
			continue ;
		}
		if (!lexer(info))
		{
			exit_lexer(info);
			continue ;
		}
		if (simple_syntax(info->tokens) == 2)
		{
			exit_simpl_syntax(info);
			continue ;
		}
		second_part_exec(info, cmds);
	}
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	t_info	info;

	(void)argv;
	if (argc != 1)
	{
		printf("This program does not accept arguments\n");
		exit(1);
	}
	if (init_struct(&info, env) == 1)
		return (-1);
	if (start_reading(&info))
	{
		if (info.copy)
			free_envlst(info.copy);
		free_envlst(info.envp);
		free_lexlst(info.tokens);
	}
	return (info.ex_stat);
}

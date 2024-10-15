/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 12:56:35 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/15 04:50:27 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "lexer.h"
#include "struct.h"
#include "parsing.h"
#include "execution.h"
#include "expansion.h"

int g_global;

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
/*
int	do_shell(char *line, t_info *info)
{
	token = do_token(line, info);
	cmd = do_cmd(token, info);
	exito = executa(cmd, info);
	return (exito);
}*/

int	start_reading(t_info *info)
{
	t_cmd	*cmds;
	int		i;

	i = 0;
	while (1)
	{
	//	if (info->ex_stat != 130 && info->ex_stat != 131 && info->ex_stat != 127 && g_global != 131 && g_global != 130)
	//	   g_global = 0;
		g_global = info->ex_stat;
		signal(SIGINT, handle_norm_sig);
		signal(SIGQUIT, SIG_IGN);
		info->prev_ex_stat = info->ex_stat;
		//info->ex_stat = g_global;
		info->rl = readline("our minishell: ");
		if (!info->rl)
		{
			if (info->copy)
				free_envlst(info->copy);
			info->copy = NULL;
			return (1);
		}
		if (info->rl[0])
			add_history(info->rl);
		/*printf("user input: %s\n", info->rl);*/
		if(!valid_line(info))
		{
			free(info->rl);
			i++;
			continue;
		}
//		info->exit = do_shell(info->rl, info);
		if (!lexer(info))
		{
	//		printf("lexer went wrong\n");
			printf("exit status: %d\n", info->ex_stat);
			printf("global: %d\n", g_global);
			free(info->rl);
			if (g_global == 130 || g_global == 131)
			{
				printf("I change ex_stat to global\n");
				info->ex_stat = g_global;
			}
			else
			{
				printf("I change ex_stat to prev_ex_stat\n");
				info->ex_stat = info->prev_ex_stat;
			}
			continue;
		}
		if (!info->tokens)
		{
			printf("tokens dont exist\n");
			free(info->rl);
			free_lexlst(info->tokens);
			info->tokens = NULL;
			g_global = info->ex_stat;
			info->ex_stat = info->prev_ex_stat;
			i++;
			continue;		
		}
//		print_lex_lst(info->tokens);
		if (simple_syntax(info->tokens) == 2)
		{
			free(info->rl);
			free_lexlst(info->tokens);
			info->tokens = NULL;
			info->ex_stat = 2;
			i++;
			printf("exit status: %d\n", info->ex_stat);
			continue;
		}
		cmds = tkn_to_cmd(info->tokens);
		if (!cmds)
		{
			free_lexlst(info->tokens);
			info->tokens = NULL;
			free(info->rl);
			exit(-1); // malloc err
		}
//		print_cmds(cmds);
		free_lexlst(info->tokens);
		info->tokens = NULL;
		info->ex_stat = executor(cmds, info);
		printf("exit status: %d\n", info->ex_stat);
	//	printf("global: %d\n", g_global);
		if (info->ex_stat == -1)
		{
			//liberar y salir
			return (-1);
		}
		free_cmds(cmds);
//		rl_clear_history(info->rl);
		free(info->rl);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	(void)argv;
	t_info	info;

	if (argc != 1)
	{
		printf("This program does not accept arguments\n");
		exit(1);
	}
	if (init_struct(&info, env) == 1)
		return (-1);
	if (start_reading(&info))
	{
		free_envlst(info.envp);
		free_lexlst(info.tokens);
	}	
	return (info.ex_stat);
}

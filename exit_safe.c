/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_safe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:51:18 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/15 13:51:39 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "lexer.h"
#include "struct.h"
#include "parsing.h"
#include "execution.h"
#include "expansion.h"

void	exit_simpl_syntax(t_info *info)
{
	free(info->rl);
	free_lexlst(info->tokens);
	info->tokens = NULL;
	info->ex_stat = 2;
}

void	exit_lexer(t_info *info)
{
	free(info->rl);
	if (g_global == 130 || g_global == 131)
		info->ex_stat = g_global;
	else
		info->ex_stat = info->prev_ex_stat;
}

void	exit_tkn_to_cmd(t_info *info)
{
	free_lexlst(info->tokens);
	info->tokens = NULL;
	free(info->rl);
	exit(-1);
}

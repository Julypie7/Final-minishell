/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:56:25 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/15 17:20:11 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "struct.h"
#include "execution.h"
#include "signal.h"
#include "expansion.h"

int	executor(t_cmd *segmts, t_info *info)
{
	t_cmd	*aux;
	t_exec	exec_info;

	signal(SIGINT, SIG_IGN);
	if (!segmts)
		return (-1);
	if (set_exec_info(info->envp, &exec_info, segmts) == 1)
		return (-1);
	aux = segmts;
	if (!start_heredoc(segmts, info, &exec_info))
		return (info->ex_stat);
	if (!segmts->next)
	{
		exec_simp_cmd(segmts, info, &exec_info);
		dup2(exec_info.or_fd[0], 0);
		dup2(exec_info.or_fd[1], 1);
		free_exec_info(&exec_info);
		return (info->ex_stat);
	}
	if (exec_mult_cmd(segmts, &exec_info, info) == 2)
		return (-1);
	mult_cmd_signal(aux, info, &exec_info);
	return (info->ex_stat);
}

int	exec_simp_cmd(t_cmd *cmd, t_info *info, t_exec *exec_info)
{
	if (prep_cmds(cmd, info, exec_info) == 2)
		return (2);
	if (cmd->arr_cmd && !find_cmd_type(cmd->arr_cmd[0]))
	{
		expand_files(cmd->redirs, info->envp, info->prev_ex_stat);
		if (builtin_cmd_protect(cmd, info) == 1)
			return (1);
		info->ex_stat = exec_builtin(cmd->arr_cmd, info, cmd, exec_info);
		return (info->ex_stat);
	}
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (1);
	if (cmd->pid == 0)
	{
		sig_def();
		simp_child_cmd(cmd, info, exec_info);
	}
	simpl_cmd_signal(cmd, info);
	return (0);
}

int	exec_mult_cmd(t_cmd *tmp, t_exec *exec_info, t_info *info)
{
	t_cmd	*cmd;

	cmd = tmp;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (cmd)
	{
		if (prep_cmds(cmd, info, exec_info) == 2)
			return (2);
		cmd->pid = fork();
		if (cmd->pid == -1)
			return (2);
		if (cmd->pid == 0)
		{
			sig_def();
			signal(SIGINT, handle_norm_sig);
			mult_child(cmd, info, exec_info);
		}
		close(exec_info->pipe_end[1]);
		if (cmd->fd_in != -1)
			close(cmd->fd_in);
		cmd = cmd->next;
	}
	close(exec_info->pipe_end[0]);
	return (0);
}

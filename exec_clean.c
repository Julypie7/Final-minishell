/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:14:49 by martalop          #+#    #+#             */
/*   Updated: 2024/10/15 12:58:39 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "execution.h"
#include "struct.h"
#include "signal.h"
#include "expansion.h"

int	fill_arr_b(char **arr_b)
{
	const char	*builtins[8];
	int			i;

	i = 0;
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	while (builtins[i])
	{
		arr_b[i] = malloc(sizeof(ft_strlen(builtins[i]) + 1));
		if (!arr_b[i])
		{
			free_array(arr_b);
			return (2);
		}
		ft_strlcpy(arr_b[i], builtins[i], ft_strlen(builtins[i]) + 1);
		i++;
	}
	arr_b[i] = NULL;
	return (1);
}

int	find_cmd_type(char *str)
{
	char	**arr_b;
	int		i;

	i = 0;
	if (!str)
		return (2);
	arr_b = malloc(sizeof(char *) * 8);
	if (!arr_b || fill_arr_b(arr_b) == 2)
		return (2);
	while (arr_b[i])
	{
		if (!ft_strncmp(str, arr_b[i], ft_strlen(arr_b[i]) + 1))
		{
			free_array(arr_b);
			return (0);
		}
		i++;
	}
	free_array(arr_b);
	return (1);
}

int	exec_builtin(char **arr_cmd, t_info *info, t_cmd *cmds, t_exec *exec_info)
{
	if (ft_strcmp(arr_cmd[0], "pwd") == 0)
		return (mini_pwd(info->envp));
	else if (ft_strcmp(arr_cmd[0], "cd") == 0)
		return (mini_cd(arr_cmd, info));
	else if (ft_strcmp(arr_cmd[0], "env") == 0)
		return (mini_env(arr_cmd, info));
	else if (ft_strcmp(arr_cmd[0], "echo") == 0)
		return (mini_echo(arr_cmd));
	else if (ft_strcmp(arr_cmd[0], "unset") == 0)
		return (mini_unset(arr_cmd, info));
	else if (ft_strcmp(arr_cmd[0], "export") == 0)
		return (mini_export(arr_cmd, info));
	else
		return (mini_exit(arr_cmd, info, cmds, exec_info));
}

int	prep_cmds(t_cmd *cmd, t_info *info, t_exec *exec_info)
{
	if (cmd->arr_cmd)
		cmd->arr_cmd = cmd_expansion(cmd->arr_cmd, info->envp, \
				info->prev_ex_stat);
	cmd->path = find_path(exec_info->paths, cmd->arr_cmd);
	if (!cmd->path)
		return (2);
	if (cmd->next)
	{
		pipe(exec_info->pipe_end);
		cmd->fd_out = exec_info->pipe_end[1];
		cmd->next->fd_in = exec_info->pipe_end[0];
	}
	return (0);
}

int	set_exec_info(t_envp *envp, t_exec *exec_info, t_cmd *segmts)
{
	exec_info->env = envlst_to_arr(envp);
	if (!exec_info->env)
		return (1);
	exec_info->paths = prep_cmd_paths(exec_info->env);
	if (!exec_info->paths)
		return (free_array(exec_info->env), 1);
	exec_info->or_fd[0] = dup(0);
	exec_info->or_fd[1] = dup(1);
	exec_info->cmd_num = cmdlst_size(segmts);
	return (0);
}

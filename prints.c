/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:05:32 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/15 12:06:01 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redirs_lst(t_redir *redirs)
{
	t_redir	*tmp;

	tmp = redirs;
	while (tmp)
	{
		printf("redir[%p]\ntoken: %d\nfile_name: %s\n\
				amb_red: %d\nfd: %d\nnext: %p\n\n",
			tmp, tmp->type, tmp->file_name, tmp->amb_red, tmp->fd, tmp->next);
		tmp = tmp->next;
	}
}

void	print_cmds(t_cmd *cmds)
{
	t_cmd	*aux;
	int		i;

	aux = cmds;
	while (aux)
	{
		printf("CMD[%p]\narr_cmd: %p\npath: %s\npid : %d\n", aux,
			aux->arr_cmd, aux->path, aux->pid);
		i = 0;
		printf("fd_in: %d, fd_out: %d\nredirs: %p\nnext: %p\n", aux->fd_in,
			aux->fd_out, aux->redirs, aux->next);
		while (aux->arr_cmd && aux->arr_cmd[i])
		{
			printf("arr_cmd[%d]: %s\n", i, aux->arr_cmd[i]);
			i++;
		}
		print_redirs_lst(aux->redirs);
		write(2, "\n", 1);
		aux = aux->next;
	}
}

void	print_char_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}
}

void	print_env(t_envp *lst)
{
	int		i;
	t_envp	*tmp;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		printf("env[%d]:\nKEY: %s\nVALUE: %s\n\n", i, tmp->key, tmp->value);
		tmp = tmp->next;
		i++;
	}
}

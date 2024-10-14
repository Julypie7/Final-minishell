/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:07:01 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/14 16:51:58 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "minishell.h"

typedef enum s_type
{
	PIPE = 1,
	OUTPUT,
	APPEND,
	INPUT,
	HEREDOC,
}	t_type;

typedef struct s_redir
{
	t_type			type;
	char			*file_name;
	int				fd;
	int				amb_red;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**arr_cmd;
	char			*path;
	int				pid;
	int				fd_in;
	int				fd_out;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_exec
{
	char	**paths;
	int		or_fd[2];
	char	**env;
	int		cmd_num;
	int		pipe_end[2];
}	t_exec;

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_lex_lst
{
	char				*word;
	t_type				type;
	char				*t_content;
	struct s_lex_lst	*next;
}	t_lex_lst;

typedef struct s_info
{
	char		*rl;
	t_lex_lst	*tokens;
	int			ex_stat;
	int			prev_ex_stat;
	t_envp		*envp;
	t_envp		*copy;
}	t_info;

#endif

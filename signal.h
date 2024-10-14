/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:41:40 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/12 12:27:37 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft/libft.h"
# include "minishell.h"

void	handle_norm_sig(int sig);
void	handle_hd_sig(int sig);
void	sig_def(void);
void	signal_handle(int status);
void	wif_herdoc(int status);

#endif

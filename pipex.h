/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoroz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:12:41 by dmoroz            #+#    #+#             */
/*   Updated: 2024/04/24 19:12:41 by dmoroz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "get_next_line_bonus.h"
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex_state
{
	int		n_cmd;
	int		here_doc;
	char	*in_path;
	char	*out_path;
	int		*pipes;
}	t_pipex_state;

void	setup(int ac, char **av, t_pipex_state *state);
void	run_cmd(t_pipex_state *state, char *cmd, int i, char **envp);
void	find_executable(char **av, char **envp);
// utils
void	clean_n_exit(t_pipex_state *state, int i,
			void (*error_func)(char *), char *err_msg);
void	clean_state(t_pipex_state *state, int i);
void	exit_with_error(char *err_msg);
void	exit_with_perror(char *err_suf);
void	free_split(char **str_arr);

#endif
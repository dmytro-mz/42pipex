/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoroz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:08:39 by dmoroz            #+#    #+#             */
/*   Updated: 2024/04/24 19:08:39 by dmoroz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	main_loop(char **av, char **envp, t_pipex_state *state);

int	main(int ac, char *av[], char **envp)
{
	t_pipex_state	state;
	int				i;
	pid_t			pid;

	setup(ac, av, &state);
	main_loop(av, envp, &state);
	clean_state(&state, i - 1);
	i = 0;
	while (i < state.n_cmd)
	{
		wait(NULL);
		i++;
	}
	if (state.here_doc)
		unlink(state.in_path);
	return (0);
}

void	main_loop(char **av, char **envp, t_pipex_state *state)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < state->n_cmd)
	{
		if (i < state->n_cmd - 1)
			if (pipe(&(state->pipes[i * 2])) == -1)
				clean_n_exit(&state, i - 1, &exit_with_perror, "pipe");
		pid = fork();
		if (pid == -1)
			clean_n_exit(&state, i, &exit_with_perror, "fork");
		if (pid == 0)
		{
			run_cmd(&state, av[2 + i + state->here_doc], i, envp);
		}
		i++;
	}
}

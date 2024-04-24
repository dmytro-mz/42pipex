#include "pipex.h"
#include <stdio.h>

void set_stdin(t_pipex_state *state, int i);
void set_stdout(t_pipex_state *state, int i);

void run_cmd(t_pipex_state *state, char *cmd, int i, char **envp)
{
    char **av;

    // printf("  PROCESS %d: Start\n", i);
    // printf("  PROCESS %d: CMD: %s\n", i, cmd);
    av = ft_split(cmd, ' ');
    find_executable(av, envp);
    // printf("  PROCESS %d: executable: %s\n", i, av[0]);
    set_stdin(state, i);
    set_stdout(state, i);
    clean_state(state, i);
    // printf("  PROCESS %d: cleaning done\n", i);
    // printf("  PROCESS %d: Running cmd\n", i);
    execve(av[0], av, envp);
    // printf("  PROCESS %d: CMD %s FAILED!!!!\n", i, av[0]);
    // printf("  PROCESS %d: CMD %s FAILED!!!!\n", i, av[1]);
    free_split(av);
    clean_n_exit(state, -1, &exit_with_perror, cmd);
}

void set_stdin(t_pipex_state *state, int i)
{
    int in_fd;
    int dup_state;

    if (i == 0)
    {
        in_fd = open(state->in_path, O_RDONLY);
        if (in_fd == -1)
            clean_n_exit(state, i, &exit_with_perror, state->in_path);
    }
    else
        in_fd = state->pipes[(i - 1) * 2];
    // printf("  PROCESS %d: IN_FD: %d\n", i, in_fd);
    dup_state = dup2(in_fd, STDIN_FILENO);
    // if (i == 0)
    //     if (close(in_fd) == -1)
    //         clean_n_exit(state, i, &exit_with_perror, "close");
    if (dup_state == -1)
        clean_n_exit(state, i, &exit_with_perror, "dup2");
}

void set_stdout(t_pipex_state *state, int i)
{
    int out_fd;
    int dup_state;

    if (i == state->n_cmd - 1)
    {
        out_fd = open(state->out_path, OUTFD_FLAGS(state->here_doc), 0777);
        if (out_fd == -1)
            clean_n_exit(state, i, &exit_with_perror, state->out_path);
    }
    else
        out_fd = state->pipes[i * 2 + 1];
    // printf("  PROCESS %d: OUT_FD: %d\n", i, out_fd);
    dup_state = dup2(out_fd, STDOUT_FILENO);
    if (i == state->n_cmd - 1)
        if (close(out_fd) == -1)
            clean_n_exit(state, i, &exit_with_perror, "close");
    if (dup_state == -1)
        clean_n_exit(state, i, &exit_with_perror, "dup2");
}

#include "pipex.h"
#include "ft_printf.h"

void set_stdin(t_pipex_state *state, int i);
void set_stdout(t_pipex_state *state, int i);

void run_cmd(t_pipex_state *state, char *cmd, int i)
{
    char **av;
    char *envp;

    ft_printf("  PROCESS %d: Start\n", i);
    ft_printf("  PROCESS %d: CMD: %s\n", i, cmd);
    set_stdin(state, i);
    ft_printf("  PROCESS %d: stdin set\n", i);
    set_stdout(state, i);
    ft_printf("  PROCESS %d: stdout set. start cleaning\n", i);
    clean_state(state, i);
    ft_printf("  PROCESS %d: cleaning done\n", i);
    av = ft_split(cmd, ' ');
    envp = NULL;
    ft_printf("  PROCESS %d: Running cmd\n", i);
    execvpe(av[0], av, &envp);
    ft_printf("  PROCESS %d: CMD %s FAILED!!!!\n", i, av[0]);
    ft_printf("  PROCESS %d: CMD %s FAILED!!!!\n", i, av[1]);
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
            clean_n_exit(state, i, &exit_with_perror, "open");
    }
    else
        in_fd = state->pipes[(i - 1) * 2];
    dup_state = dup2(in_fd, STDIN_FILENO);
    if (i == 0)
        if (close(in_fd) == -1)
            clean_n_exit(state, i, &exit_with_perror, "close");
    if (dup_state == -1)
        clean_n_exit(state, i, &exit_with_perror, "dup2");
}

void set_stdout(t_pipex_state *state, int i)
{
    int out_fd;
    int dup_state;

    if (i == state->n_cmd - 1)
    {
        out_fd = open(state->out_path, OUTFD_FLAGS(state->here_doc), S_777);
        if (out_fd == -1)
            clean_n_exit(state, i, &exit_with_perror, "open");
    }
    else
        out_fd = state->pipes[i * 2 + 1];
    dup_state = dup2(out_fd, STDOUT_FILENO);
    if (i == state->n_cmd - 1)
        if (close(out_fd) == -1)
            clean_n_exit(state, i, &exit_with_perror, "close");
    if (dup_state == -1)
        clean_n_exit(state, i, &exit_with_perror, "dup2");
}

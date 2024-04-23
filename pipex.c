#include "pipex.h"

int main(int ac, char *av[])
{
    t_pipex_state state;
    int i;
    pid_t pid;

    setup(ac, av, &state);
    i = 0;
    while (i < state.n_cmd)
    {
        if (i < state.n_cmd - 1)
            if (pipe(&(state.pipes[i * 2])) == -1)
                clean_n_exit_with_error(strerror(errno), &state, i - 1);
        pid = fork();
        if (pid == -1)
            clean_n_exit_with_error(strerror(errno), &state, i);
        if (pid == 0)
            run_cmd(&state, av[2 + i + state.here_doc], i);
        i++;
    }
    clean_state(&state, i - 1);
    while (i-- > 0)
        wait(NULL);
    if (state.here_doc)
        unlink(state.in_path);
    return (0);
}

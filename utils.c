#include "pipex.h"

void clean_n_exit_with_error(char *err, t_pipex_state *state, int i)
{
    clean_state(state, i);
    exit_with_error(err);
}

void clean_state(t_pipex_state *state, int i)
{
    int j;

    if (i >= 0)
    {
        j = 0;
        while (j < (i + 1) * 2)
            close(state->pipes[j]);
    }
    free(state->pipes);
}

void exit_with_error(char *err)
{
    ft_putendl_fd(err, STDERR_FILENO);
    exit(1);
}

void free_split(char **str_arr)
{
    int i;

    i = 0;
    while (str_arr[i])
        free(str_arr[i]);
    free(str_arr);
}

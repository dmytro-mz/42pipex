#include "pipex.h"
#include <stdio.h>

void clean_n_exit(t_pipex_state *state, int i, void(*error_func)(char *), char *err_msg)
{
    // printf("  PROCESS %d: clean_n_exit: %s\n", i, err_msg);
    clean_state(state, i);
    (*error_func)(err_msg);
}

void clean_state(t_pipex_state *state, int i)
{
    int j;

    if (i == state->n_cmd - 1)
        i--;
    if (i >= 0)
    {
        j = 0;
        while (j < (i + 1) * 2)
            close(state->pipes[j++]);
    }
    free(state->pipes);
    state->pipes = NULL;
}

void exit_with_error(char *err_msg)
{
    ft_putendl_fd(err_msg, STDERR_FILENO);
    exit(1);
}

void exit_with_perror(char *err_suf)
{
    perror(err_suf);
    exit(1);
}

void free_split(char **str_arr)
{
    int i;

    i = 0;
    while (str_arr[i])
        free(str_arr[i++]);
    free(str_arr);
    str_arr = NULL;
}

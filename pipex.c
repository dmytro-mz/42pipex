#include "pipex.h"

int main(int ac, char *av[])
{
    t_pipex_state state;
    int i;
    pid_t pid;

    main_setup(ac, av, &state);
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
    while (i > 0)
        wait();
    return (0);
}

void main_setup(int ac, char **av, t_pipex_state *state)
{
    state->here_doc = 0;
    if (ac < 5)
        exit_with_error("Wrong number of parameters!");
    if (ft_memcmp(av[1], "here_doc", 9) == 0)
    {
        state->here_doc = 1;
        if (ac < 6)
            exit_with_error("Wrong number of parameters!");
    }
    state->n_cmd = ac - 3 - state->here_doc
    state->in_fd = get_in_fd(av, state->here_doc);
    state->out_fd = open(av[ac - 1], OUTFD_FLAGS(state->here_doc), S_777);
    state->pipes = malloc((state->n_cmd - 1) * 2 * sizeof(int));
    if (!state->pipes)
        clean_n_exit_with_error(strerror(errno), state, -1);
}

void exit_with_error(char *str)
{
    ft_putendl_fd(str, STDERR_FILENO);
    exit(1);
}

int get_in_fd(char **av, int is_here_doc)
{
    int rd;

    if (is_here_doc)
        rd = get_in_fd_here_doc(av);
    else
        rd = open(av[1], O_RDONLY);
    if (rd == -1)
        ft_putendl_fd(strerror(errno), STDERR_FILENO);
    return (rd);
}

int get_in_fd_here_doc(char **av)
{
    int wr;
    int lim_len;
    char *line;

    lim_len = ft_strlen(av[1]);
    wr = open("__tmptodrop", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (wr == -1)
        exit_with_error("Failed to create tmp file.");
    line = get_next_line(STDIN_FILENO);
    while (line && !(ft_memcmp(line, av[1], lim_len) && line[lim_len] == '\n'))
    {
        ft_putstr_fd(line, wr);
        free(line); 
        line = get_next_line(wr);
    }
    free(line);
    close(wr);
    return (open("__tmptodrop", O_RDONLY));
}

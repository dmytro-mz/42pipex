#include "pipex.h"

char *get_in_fd(char **av, int is_here_doc);
char *get_in_fd_here_doc(char *lim);

void setup(int ac, char **av, t_pipex_state *state)
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
    state->n_cmd = ac - 3 - state->here_doc;
    state->in_path = get_in_fd(av, state->here_doc);
    state->out_path = av[ac - 1];
    state->pipes = malloc((state->n_cmd - 1) * 2 * sizeof(int));
    if (!state->pipes)
        clean_n_exit(state, -1, &exit_with_perror, "malloc");
}

char *get_in_fd(char **av, int is_here_doc)
{
    if (is_here_doc)
        return (get_in_fd_here_doc(av[2]));
    else
        return (av[1]);
}

char *get_in_fd_here_doc(char *lim)
{
    int wr;
    size_t lim_len;
    char *line;

    lim_len = ft_strlen(lim);
    wr = open("__tmp_file", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (wr == -1)
        exit_with_perror("__tmp_file");
    line = get_next_line(STDIN_FILENO);
    while (line && !(!ft_memcmp(line, lim, lim_len) && line[lim_len] == '\n'))
    {
        ft_putstr_fd(line, wr);
        free(line); 
        line = get_next_line(STDIN_FILENO);
    }
    free(line);
    close(wr);
    return ("__tmp_file");
}

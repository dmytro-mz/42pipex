#include "pipex.h"
#include "ft_printf.h"

int main(int ac, char *av[], char **envp)
{
    t_pipex_state state;
    int i;
    pid_t pid;
    
    ft_printf("Start\n");
    setup(ac, av, &state);
    ft_printf("setup done\n");
    i = 0;
    while (i < state.n_cmd)
    {
        ft_printf("Running %d cmd...\n", i);
        if (i < state.n_cmd - 1)
            if (pipe(&(state.pipes[i * 2])) == -1)
                clean_n_exit(&state, i - 1, &exit_with_perror, "pipe");
        pid = fork();
        if (pid == -1)
            clean_n_exit(&state, i, &exit_with_perror, "fork");
        if (pid == 0){
            ft_printf("%d: %s\n", 2 + i + state.here_doc, av[2 + i + state.here_doc]);
            run_cmd(&state, av[2 + i + state.here_doc], i);}
        i++;
    }
    ft_printf("All cmd run. Start cleanning\n", i);
    clean_state(&state, i - 1);
    ft_printf("Cleanning done. Waiting for forks...\n", i);
    while (i-- > 0)
        wait(NULL);
    ft_printf("All forkes finished work\n", i);
    if (state.here_doc)
        unlink(state.in_path);
    return (0);
}

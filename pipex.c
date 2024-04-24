#include "pipex.h"
#include <stdio.h>

int main(int ac, char *av[], char **envp)
{
    t_pipex_state state;
    int i;
    pid_t pid;
    
    // printf("Start\n");
    setup(ac, av, &state);
    // printf("setup done\n");
    i = 0;
    while (i < state.n_cmd)
    {
        // printf("Running %d cmd...\n", i);
        if (i < state.n_cmd - 1)
            if (pipe(&(state.pipes[i * 2])) == -1)
                clean_n_exit(&state, i - 1, &exit_with_perror, "pipe");
        pid = fork();
        if (pid == -1)
            clean_n_exit(&state, i, &exit_with_perror, "fork");
        if (pid == 0){
            // printf("%d: %s\n", 2 + i + state.here_doc, av[2 + i + state.here_doc]);
            run_cmd(&state, av[2 + i + state.here_doc], i, envp);}
        i++;
    }
    // printf("All cmd run. Start cleanning\n");
    clean_state(&state, i - 1);
    // printf("Cleanning done. Waiting for forks...\n");
    while (i-- > 0)
        wait(NULL);
    // printf("All forkes finished work\n");
    if (state.here_doc)
        unlink(state.in_path);
    return (0);
}

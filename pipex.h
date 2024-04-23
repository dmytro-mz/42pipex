#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include "get_next_line_bonus.h"
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

# define S_777 S_IRWXU | S_IRWXG | S_IRWXO
# define OUTFD_FLAGS(f) O_WRONLY | O_CREAT | (O_APPEND * (f)) | (O_TRUNC * (!(f)))

typedef struct  s_pipex_state
{
    int n_cmd;
    int here_doc;
    char *in_path;
    char *out_path;
    int *pipes;
} t_pipex_state;

void setup(int ac, char **av, t_pipex_state *state);
void run_cmd(t_pipex_state *state, char *cmd, int i);
void clean_n_exit_with_error(char *err, t_pipex_state *state, int i);
void clean_state(t_pipex_state *state, int i);
void exit_with_error(char *err);
void free_split(char **str_arr);

#endif
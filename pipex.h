#ifndef PIPEX_H
# define PIPEX_H

# include "libft"
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

# define S_777 S_IRWXU | S_IRWXG | S_IRWXO
# define OUTFD_FLAGS(f) O_WRONLY | O_CREAT | (O_APPEND * (f)) | (O_TRUNC * (!(f)))

typedef struct  s_pipex_state
{
    int n_cmd;
    int here_doc;
    int in_fd;
    int out_fd;
    int *pipes;
} t_pipex_state;


#endif
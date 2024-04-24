#include "pipex.h"
#include <stdio.h>

char **get_path(char **envp);
char *find_location(char **path, char *exec_name);
int is_executable(char *exec_path, char *exec_name);
char *ft_pathjoin(char *exec_path, char *exec_name);

void find_executable(char **av, char **envp)
{
    char **path;
    char *location;
    char *new_exec;

    // printf("In find_executable\n");
    // printf("av[0]: %s\n", av[0]);
    if (av[0][0] == '/')
        return ;
    // printf(":)\n", av[0]);
    path = get_path(envp);
    // printf("path: %p\n", path);
    if (!path)
        return ;
    location = find_location(path, av[0]);
    // printf("location: %s\n", location);
    if (location)
    {
        new_exec = ft_pathjoin(location, av[0]);
        if (new_exec)
        {
            free(av[0]);
            av[0] = new_exec;
        }
    }
    // printf("av[0]: %s\n", av[0]);
    free_split(path);
    // printf("OUT find_executable\n");
}

char **get_path(char **envp)
{
    int i;
    char **path;

    path = NULL;
    // printf("Searching PATH\n");
    i = 0;
    while (envp[i])
    {
        // printf("Maybe: %s\n", envp[i]);
        if (ft_memcmp(envp[i], "PATH=", 5) == 0)
        {
            path = ft_split(envp[i] + 5, ':');
            break ;
        }
        i++;
    }
    return (path);
}

char *find_location(char **path, char *exec_name)
{
    int i;

    i = 0;
    while (path[i])
    {
        // printf("Checking: %s/%s\n", path[i], exec_name);
        if (is_executable(path[i], exec_name))
            return (path[i]);
        i++;
    }
    return (NULL);
}

int is_executable(char *exec_path, char *exec_name)
{
    int ret_val;
    char *full_path;

    full_path = ft_pathjoin(exec_path, exec_name);
    if (!full_path)
        return (0);
    ret_val = 0;
    if (access(full_path, X_OK) == 0)
        ret_val = 1;
    // printf("Result of %s: %d\n", full_path, ret_val);
    free(full_path);
    return (ret_val);
}

char *ft_pathjoin(char *exec_path, char *exec_name)
{
    char *full_path;
    size_t path_size;
    size_t name_size;
    
    path_size = ft_strlen(exec_path);
    name_size = ft_strlen(exec_name);
    full_path = malloc(path_size + name_size + 2);
    if (!full_path)
        return (NULL);
    ft_memmove(full_path, exec_path, path_size);
    full_path[path_size] = '/';
    ft_memmove(full_path + path_size + 1, exec_name, name_size);
    full_path[path_size + name_size + 1] = 0;
    return (full_path);
}

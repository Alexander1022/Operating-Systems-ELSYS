#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

char** parse_cmdline(const char* cmdline)
{
    int index = 0;
    int arguments = 0;
    char** divided_please = malloc(2 * sizeof(char*)); 

    while(1)
    {
        int id = 0;
        if(cmdline[0] == '\n')
        {
            divided_please[0] = malloc(1);
            divided_please[arguments][id] = '\0';
        }

        else
        {
            if(cmdline[index] == ' ')
            {
                divided_please[arguments][id] = '\0';
                divided_please[arguments] = realloc(divided_please[arguments], (arguments + 2) * sizeof(char*));
                arguments++;
                id = 0;
            }

            else if(cmdline[index] != ' ')
            {
                divided_please[arguments][id] = cmdline[index];
                id++;
            }
        }
    }

    return divided_please;
}

int main()
{
    char* input = malloc(2);
    size_t size = 0;
    size_t read_f;
    
    write(1, "$ ", 2);

    while(input[size - 1] != '\n')
    {
        read_f = read(0, input + size, 1); 
        size = size + 1; 
        input = realloc(input, 1 + size);
    }

    char** argv_list = parse_cmdline(input); 
    
    int path_size = 0;

    for( ; input[path_size] != ' ' ; path_size++);
    
    char* path = malloc(path_size);
    
    for(int s = 0 ; s < path_size ; s++)
    {
        path[s] = input[s];
    }

    pid_t f = fork();

    if(f == -1)
    {
        perror("fork");
    }

    else if(f == 0)
    {
        int exec = execv(path, argv_list);

        if(exec == -1)
        {
            perror(path);
        }
    }

    else
    {
        int status;
        waitpid(f, &status, 0);
    }

    free(argv_list);
    free(input);
}
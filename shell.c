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
    int ind = 0;
    int arguments = 0;
    char** divided_please = (char **)malloc(2 * sizeof(char));

    for(int index = 0 ; index < strlen(cmdline) ; index++)
    {
        if(cmdline[0] == '\n')
        {
            divided_please[0] = malloc(1);
            divided_please[0][0] =  '\0';
        }

        else
        {
            if(cmdline[index] == ' ')
            {
                divided_please[arguments][ind] = '\0';
                divided_please[arguments] = realloc(divided_please[arguments], (arguments + 2) * sizeof(char*));
                arguments++;
                ind = 0;
            }

            else if(cmdline[index] != ' ' && cmdline[index] != '\n')
            {
                divided_please[arguments] = realloc(divided_please[arguments], ind + 2);
                divided_please[arguments][ind] = cmdline[index];
                ind ++;
            }
        }
    }

    return divided_please;
}

int main()
{
    char* input;
    size_t size = 100;
    int path_size = 0;

    input = (char *)malloc(size * sizeof(char));
    
    write(1, "$ ", 2);

    getline(&input, &size, stdin);

    int i = 0;

    for( ; i < strlen(input) ; i++)
    {
        if(input[i] == ' ')
        {
            break;
        }
    }

    path_size = i;

    char* path = malloc(path_size);
    
    for(int i = 0 ; i < path_size ; i++)
    {
        path[i] = input[i];
    }

    char** argv_list = parse_cmdline(input); 

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
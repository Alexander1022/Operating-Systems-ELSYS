#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


char** parse_cmdline(const char* cmdline)
{
    int size_new = strlen(cmdline);
    int j = 0;
    int arguments = 0;
    char** divided_please = malloc( );

    for (int i = 0 ; i < size_new ; i++)
    {
        if(cmdline[i] == ' ' || cmdline[i] == '\0')
        {
            divided_please[arguments][j] = '\0';
            arguments++;
            j = 0;
        }

        else 
        {
            divided_please[arguments][j] = cmdline[i];
            j++;
        }
    }

    return divided_please;
}

int main()
{
    char* input = malloc(1);
    size_t size = 0;
    size_t read_it = 0;
    
    while(input[size - 1] != '\n')
    {
        read_it = read(0, input + size, 1); 
        size = size + read_it; 
        input = realloc(input, 1 + size);
    }

    char** argv_list = parse_cmdline(input);

    pid_t f = fork();
    char* path = argv_list[0];
    execv(path, argv_list);
}
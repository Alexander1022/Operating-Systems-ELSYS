//------------------------------------------------------------------------
// NAME: Alexander Yordanov
// CLASS: XIb
// NUMBER: 1
// PROBLEM: 2
// FILE NAME: shell.c
// FILE PURPOSE:
// Целта на задачата е да се реализира прост команден интерпретатор shell.
// При стартиране на програмата, тя започва да чете редове от стандартния вход и да ги интерпретира. 
// За тази цел програмата трябва да раздели командния ред на думи, като за разделител се използва интервал (’ ’).
//------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//------------------------------------------------------------------------
// FUNCTION: parse_cmdline 
// предназначение на функцията: разделя динамичен масив на отделни думи, където има разстояние
// PARAMETERS: const char* cmdline (предназначение - stdin)
//------------------------------------------------------------------------

char** parse_cmdline(const char* cmdline)
{
    int ind = 0;
    int arguments = 0;
    char** divided_please = malloc(2 * sizeof(char));

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
                divided_please[arguments] = realloc(divided_please[arguments], (arguments + 2) * sizeof(char));
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

    arguments ++;
    divided_please[arguments] = NULL;

    return divided_please;
}

//------------------------------------------------------------------------
// FUNCTION: main
// предназначение на функцията: execute-ва програмата, дадена от нас в stdin
// PARAMETERS: -
//------------------------------------------------------------------------

int main()
{
    while(1)
    {
        char* input;
        size_t size = 100;
        int path_size = 0;
        int bytes_read;

        input = (char *)malloc(size * sizeof(char));
        
        write(1, "$ ", 2);

        bytes_read = getline(&input, &size, stdin);

        if(strlen(input) == 1)
        {
            continue;
        }

        if(bytes_read == -1)
        {
            write(1, "\n", 1);
            break;
        }

        //just in case i need path separately. 
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
        
        for(int j = 0 ; j < path_size ; j++)
        {
            path[j] = input[j];
        }

        char** argv_list = parse_cmdline(input);

        pid_t f = fork();

        if(f == -1)
        {
            perror("fork");
        }

        else if(f == 0)
        {
            int exec = execv(argv_list[0], argv_list);

            if(exec == -1)
            {
                perror(argv_list[0]);
            }

            exit(f);
        }

        else
        {
            int status = 0;
            int pid = waitpid(f, &status, 0);

            if(pid < 0)
            {
                perror("waitpid");
                break;
            }

            if(pid == 0)
            {
                break;
            }
        }

        free(argv_list);
        free(input);
        free(path);
    }
}
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
    int arguments = 0;
    char* copy_of_cmdline = strdup(cmdline);
    char** divided_please = (char**)malloc(1 * sizeof(char*));
    char* arr = strtok(copy_of_cmdline, " ");
    int i;

    while(arr != NULL)
    {
        int size = strlen(arr);
        int index = 0;
        i = 0;

        while(i != size)
        {
            if(arr[i] == '\n')
            {
                break;
            }

            else
            {
                arr[index++] = arr[i];
            }
            
            i++;
        }

        arr[index] = '\0';

        divided_please[arguments] = arr;
        arguments ++;
        divided_please = realloc(divided_please, (arguments + 1) * sizeof(char*));
        
        arr = strtok(NULL, " ");
    }

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
        size_t size = 150;
        int path_size = 0;
        int bytes_read;

        input = (char *)malloc(size * sizeof(char));
        
        write(1, "$ ", 2);

        bytes_read = getline(&input, &size, stdin);

        if(bytes_read == 1)
        {
            free(input);
            continue;
        }

        if(bytes_read == -1)
        {
            free(input);
            break;
        }

        //exec the command and the argument from parse_cmdline()
        
        pid_t f = fork();

        if(f == -1)
        {
            perror("fork");
            free(input);
        }

        else if(f == 0)
        {
            char** argv_list = parse_cmdline(input);

            int exec = execvp(argv_list[0], argv_list);

            if(exec == -1)
            {
                perror(argv_list[0]);
            }

            free(argv_list);
            free(input);
            exit(f);
        }

        else
        {
            int status = 0;
            int pid = waitpid(f, &status, 0);
            
            if(pid == -1)
            {
                perror("waitpid");
            }

            free(input);
        }
        ////
    }

    return 0;
}
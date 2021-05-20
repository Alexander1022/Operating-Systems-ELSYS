#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int args_line = 0;

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
        args_line ++;
        divided_please = realloc(divided_please, (arguments + 1) * sizeof(char*));
        
        arr = strtok(NULL, " ");
    }

    divided_please[arguments] = NULL;
    return divided_please;
}

int main(int argc, char* argv[])
{
    while(1)
    {
        char* input;
        size_t size = 150;
        int path_size = 0;
        int bytes_read;

        input = (char *)malloc(size * sizeof(char));
        
        printf("$ ");

        bytes_read = getline(&input, &size, stdin);

        if(bytes_read != -1)
        {
            char** argv_list = parse_cmdline(input);

            /*for(int s = 0 ; s < args_line ; s++)
            {
                printf("%s ", argv_list[s]);
            }*/
            if(strcmp(argv_list[0], "ls") == 0)
            {
                DIR *mydir;
                struct dirent *myfile;
                struct stat mystat;

                char cwd[PATH_MAX];

                if (getcwd(cwd, sizeof(cwd)) != NULL) 
                {
                    mydir = opendir(cwd);
                    while((myfile = readdir(mydir)) != NULL)
                    {
                        stat(myfile->d_name, &mystat); 

                        if(S_ISREG(mystat.st_mode))
                        {
                            printf("%s ", myfile->d_name);
                        }   
                    }
                }
            }

            printf("\n");
            args_line = 0;

            free(input);
            free(argv_list);
        }

        else if(bytes_read == -1)
        {
            free(input);
            break;
        }

    }

    return 0;
}
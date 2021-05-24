//--------------------------------------------
// NAME: Alexander Yordanov
// CLASS: XIb
// NUMBER: 11
// PROBLEM: #4
// FILE NAME: ls.c (unix file name)
// FILE PURPOSE:
// имплементиране на ls
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

// игртаят ролята на booleans, ако е подаден флаг става 1
int flag_A = 0;
int flag_l = 0;
int flag_R = 0;

void typeOfFile(struct stat givenFile)
{
    switch(givenFile.st_mode & S_IFMT)
    {
        //обикновен файл - ’-’
        case S_IFREG:
            printf("- ");
            break;
        //директория - ’d’
        case S_IFDIR:
            printf("d ");
            break;
        //блоково устройство - ’b
        case S_IFBLK:
            printf("b ");
            break;
        //символно устройство - ’c’
        case S_IFCHR:
            printf("c ");
            break;
        //сокет - ’s’
        case S_IFSOCK:
            printf("s ");
            break;
        //символна връзка - ’l’
        case S_IFLNK:
            printf("l ");
            break;
        //програмен поток - ’p
        case S_IFIFO:
            printf("p ");
            break;
    }
}

char* plsMakePath(char* a, char* b, char* c)
{
    size_t s1 = strlen(a);
    size_t s2 = strlen(b);
    size_t s3 = strlen(c);

    char* path = malloc(s1 + s2 + s3 + 1);

    if(path)
    {
        memcpy(path, a, s1);
        memcpy(path + s1, b, s2);
        memcpy(path + s1 + s2, c, s3 + 1);
    }

    return path;
}

int basic(char* d)
{
    DIR* folder = opendir(d);
    struct dirent* entry;
    struct stat file;

    if(!folder)
    {

        if(errno == ENOENT)
        {
            printf("ls: cannot access %s: No such file or directory\n", d);
        }

        else
        {
            printf("ls: cannot open directory %s : Permission denied\n", d);
        }
        return 0;
    }

    else
    {
        while((entry = readdir(folder)) != NULL)
        {
            if(entry->d_name[0] != '.')
            {
                char* path = plsMakePath(d, "/", entry->d_name);
                stat(path, &file);
                typeOfFile(file);
                printf("%s\n", entry->d_name); 
            }
        }
    }

    closedir(folder);
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        basic(".");
    }

    else if(argc > 1)
    {
        for(int i = 1 ; i < argc ; i++)
        {
            if(argv[i][0] == '-')
            {
                for(int a = 0; a < strlen(argv[i]) ; a++)
                {
                    if(argv[i][a] == 'A')
                    {
                       flag_A = 1;
                    }

                    else if(argv[i][a] == 'l')
                    {
                        flag_l = 1;
                    }

                    else if(argv[i][a] == 'R')
                    {
                        flag_R = 1;
                    }
                }
            }
        }

        for(int i = 1 ; i < argc ; i++)
        {
            if(argv[i][0] != '-')
            {
                basic(argv[i]);
            }
        }

        //printf("flag -A: %d\nflag -l: %d\nflag -R: %d\n", flag_A, flag_l, flag_R);
    }

    return 0;
}
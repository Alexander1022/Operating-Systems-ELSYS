//--------------------------------------------
// NAME: Alexander Yordanov
// CLASS: XIb
// NUMBER: 1
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
#include <time.h>
#include <grp.h>
#include <pwd.h>

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

int plsGiveMeSize(char* d)
{
    int blocksSize = 0;
    struct dirent* entry;
    struct stat file;
    DIR* folder = opendir(d);

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

    while((entry = readdir(folder)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char* path = plsMakePath(d, "/", entry->d_name);
            stat(path, &file);

            blocksSize = blocksSize + file.st_blocks;

            free(path);
        }
    }

    blocksSize = blocksSize / 2;
    closedir(folder);

    return blocksSize;
    
}

char* plsGiveMePerms(struct stat file)
{
    char* perms = malloc(10);

    if(S_ISDIR(file.st_mode))
    {
        perms[0] = 'd';
    }

    else
    {
        perms[0] = '-';
    }

    if(file.st_mode & S_IRUSR)
    {
        perms[1] = 'r';
    }

    else
    {
        perms[1] = '-';
    }

    if(file.st_mode & S_IWUSR)
    {
        perms[2] = 'w';
    }

    else
    {
        perms[2] = '-';
    }

    if(file.st_mode & S_IXUSR)
    {
        perms[3] = 'x';
    }

    else
    {
        perms[3] = '-';
    }

    if(file.st_mode & S_IRGRP)
    {
        perms[4] = 'r';
    }

    else
    {
        perms[4] = '-';
    }

    if(file.st_mode & S_IWGRP)
    {
        perms[5] = 'w';
    }

    else
    {
        perms[5] = '-';
    }

    if(file.st_mode & S_IXGRP)
    {
        perms[6] = 'x';
    }

    else
    {
        perms[6] = '-';
    }

    if(file.st_mode & S_IROTH)
    {
        perms[7] = 'r';
    }

    else
    {
        perms[7] = '-';
    }

    if(file.st_mode & S_IWOTH)
    {
        perms[8] = 'w';
    }

    else
    {
        perms[8] = '-';
    }

    if(file.st_mode & S_IXOTH)
    {
        perms[9] = 'x';
    }

    else
    {
        perms[9] = '-';
    }

    perms[10] = '\0';

    return perms;
}

int show(char* d)
{
    DIR* folder = opendir(d);
    struct dirent* entry;
    struct stat file;
    int size = plsGiveMeSize(d);
    int totalPrintCounter = 0;

    if(!folder)
    {

        if(errno == ENOENT)
        {
            char e[100];
            strcpy(e, "ls: cannot access ");
            strcat(e, d);
            perror(e);
        }

        else
        {
            char e[100];
            strcpy(e, "ls: cannot open directory ");
            strcat(e, d);
            perror(e);
        }
        return 0;
    }

    else
    {
        while((entry = readdir(folder)) != NULL)
        {
            // игнорираме "." и ".."
            if(strcmp(entry->d_name, "..") != 0 || strcmp(entry->d_name, ".") != 0)
            {
                if(entry->d_name[0] != '.')
                {
                    if(flag_l == 0)
                    {
                        char* path = plsMakePath(d, "/", entry->d_name);
                        stat(path, &file);
                        typeOfFile(file);
                        printf("%s\n", entry->d_name); 

                        free(path);
                    }

                    else if(flag_l == 1)
                    {
                        char* path = plsMakePath(d, "/", entry->d_name);
                        stat(path, &file);
                        
                        if( totalPrintCounter == 0)
                        {
                            totalPrintCounter = 1;
                            printf("total %d\n", size);
                        }

                        struct passwd* userInfo = getpwuid(file.st_uid);
                        struct group* grouInfo = getgrgid(file.st_gid);

                        char* perms = plsGiveMePerms(file);
                        printf("%s ", perms);
                        printf("%ld ", file.st_nlink);
                        printf("%s ", userInfo->pw_name);
                        printf("%s ", grouInfo->gr_name);
                        printf("%ld ", file.st_size);

                        char date[20];
                        strftime(date, 20, "%b %d %H:%M", localtime(&(file.st_ctime)));
                        printf("%s ", date);
                        printf("%s\n", entry->d_name);

                        free(perms);
                        free(path);
                    }
                }

                else if(flag_A == 1) 
                {
                    if(flag_l == 1)
                    {
                        if(strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0)
                        {
                            char* path = plsMakePath(d, "/", entry->d_name);
                            stat(path, &file);
                            
                            if( totalPrintCounter == 0)
                            {
                                totalPrintCounter = 1;
                                printf("total %d\n", size);
                            }

                            struct passwd* userInfo = getpwuid(file.st_uid);
                            struct group* grouInfo = getgrgid(file.st_gid);

                            char* perms = plsGiveMePerms(file);
                            printf("%s ", perms);
                            printf("%ld ", file.st_nlink);
                            printf("%s ", userInfo->pw_name);
                            printf("%s ", grouInfo->gr_name);
                            printf("%ld ", file.st_size);

                            char date[20];
                            strftime(date, 20, "%b %d %H:%M", localtime(&(file.st_ctime)));
                            printf("%s ", date);
                            printf("%s\n", entry->d_name);

                            free(perms);
                            free(path);
                        }
                    }

                    else
                    {
                        if(strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0)
                        {
                            char* path = plsMakePath(d, "/", entry->d_name);
                            stat(path, &file);
                            typeOfFile(file);
                            printf("%s\n", entry->d_name); 
                            free(path); 
                        }
                    }
                }
            }
        }
    }

    closedir(folder);
    return 0;
}

int main(int argc, char* argv[])
{
    struct stat file; 
    int arguments = 0;
    int nn = 0;

    // само ls, без аргументи, без флагове
    if(argc == 1)
    {
        show(".");
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
                arguments ++;
            }
        }

        /*if(flag_R == 1)
        {
            printf(".:\n");
        }*/

        for(int i = 1 ; i < argc ; i++)
        {
            if(argv[i][0] != '-')
            {
                if(stat(argv[i], &file)!= 0)
                {
                    char err[100];
                    strcpy(err, "ls: cannot access ");
                    strcat(err, argv[i]);
                    perror(err);
                }

                else if((file.st_mode & S_IFDIR))
                {
                    if(arguments == 1)
                    {
                        show(argv[i]);
                    }

                    else
                    {
                        if(nn == 1)
                        {
                            printf("\n");
                        }

                        printf("%s:\n", argv[i]);
                        show(argv[i]);
                        nn = 0;
                        
                        if(i != arguments)
                        {
                            printf("\n");
                        }
                    }   
                }

                else
                {
                    typeOfFile(file);
                    printf("%s\n", argv[i]);
                    nn = 1;

                    if(nn == 1)
                    {
                        continue;
                    }

                    else if(i != arguments)
                    {
                        printf("\n");
                    }
                }

                //show(argv[i]);
            }

            else
            {
                show(".");
            }
        }

        //printf("flag -A: %d\nflag -l: %d\nflag -R: %d\n", flag_A, flag_l, flag_R);
    }

    return 0;
}
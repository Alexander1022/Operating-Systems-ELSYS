//--------------------------------------------
// NAME: Alexander Jordanov
// CLASS: XI b
// NUMBER: 1
// PROBLEM: #2 - threads
// FILE NAME: starcraft.c (unix file name)
// FILE PURPOSE:
// Първата ГОЛЯМА задача за този срок - симулиране на играта Starcraft с помощта на нишки 
// Студиото за разработка на игри Blizzard Entertainment има нужда от помощ в разработването на прототип на следваща игра от поредицата StarCraft. Подобно на предишните версии
// на играта, когато играта стартира, играчът разполага с 5 работници (SCV), Команден център (Command center) и 0 минерали. Целта на играча е да построи 20 войника. Войниците
// струват минерали, които се набавят с помощта на работници. Броят на минералните блокове се подава като аргумент при стартирането на играта. Играта приключва след като се
// изкопаят всички минерални блокове на картата и се построят 20 войника. Играчът подава команди на стандартния вход. Целта на задачата е да се реализира синхронизация на
// нишки посредством mutex-и.
//---------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>


struct CommandCenter
{
    int all_the_scvs_mined;
    int soldier_counter;
};

pthread_mutex_t lock;
pthread_mutex_t soldier;

int minerals = 0;

int *blocks;
bool *flag_if_the_mineral_is_busy;
int *size_of_blocks;

struct CommandCenter cc;

void *scv(void *args)
{
    int scv_counter;
    scv_counter = *((int*)args);

    sleep(3);

    for(int i = 0 ; i < *size_of_blocks ; i++)
    {
        sleep(3);

        while(*(blocks + i) > 8)
        {
            sleep(3);
            if(*(blocks + i) > 8)
            {
                if(*(flag_if_the_mineral_is_busy + i) == 0)
                {
                    pthread_mutex_lock(&lock);
                    printf("SCV %d is mining from mineral block %d\n", scv_counter, i + 1);
                    flag_if_the_mineral_is_busy[i] = 1;
                    blocks[i] = blocks[i] - 8;
                    flag_if_the_mineral_is_busy[i] = 0;
                    printf("SCV %d is transporting minerals\n", scv_counter);
                    sleep(2);
                    printf("SCV %d delivered minerals to the Command center\n", scv_counter);
                    cc.all_the_scvs_mined = cc.all_the_scvs_mined + 8;

                    pthread_mutex_unlock(&lock);
                }


                else
                {
                    break;
                }
            }

            else
            {
                break;
            }
        }  
    }
    pthread_exit(NULL);
}

void *input(void *args)
{
    char choice;

    choice = getc(stdin);

    if(choice != '\n' && choice == 'm')
    {
        if(cc.all_the_scvs_mined > 50)
        {
            sleep(1);

            pthread_mutex_lock(&soldier);
            cc.soldier_counter = cc.soldier_counter + 1;
            pthread_mutex_unlock(&soldier);

            pthread_mutex_lock(&lock);
            cc.all_the_scvs_mined = cc.all_the_scvs_mined - 50; 
            pthread_mutex_unlock(&lock);

            printf("You wanna piece of me, boy?\n");
        }

        else
        {
            printf("Not enough minerals.\n");
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    cc.all_the_scvs_mined = 0;
    pthread_t *SCV_T = malloc(5 * sizeof(pthread_t));
    //pthread_t input_t;

    int l_rc;

    int all_minerals = 0;

    if(argc < 2)
    {
        size_of_blocks = malloc(sizeof(int));
        *size_of_blocks = 2;

        blocks = malloc(*size_of_blocks * sizeof(int));
        flag_if_the_mineral_is_busy = malloc(*size_of_blocks * sizeof(bool));

        for(int i = 0 ; i < *size_of_blocks ; i++)
        {
            blocks[i] = 500;
            all_minerals = all_minerals + blocks[i];
        }

        for(int i = 0 ; i < *size_of_blocks ; i++)
        {
            flag_if_the_mineral_is_busy[i] = 0;
        }
    }

    else if(argc == 2)
    {
        size_of_blocks = malloc(sizeof(int));
        *size_of_blocks = atoi(argv[1]);

        blocks = malloc(*size_of_blocks * sizeof(int));
        flag_if_the_mineral_is_busy = malloc(*size_of_blocks * sizeof(bool));

        for(int i = 0 ; i < *size_of_blocks ; i++)
        {
            all_minerals = all_minerals + 500;
        }

        for(int i = 0 ; i < *size_of_blocks ; i++)
        {
            flag_if_the_mineral_is_busy[i] = 0;
        }
    }

    /*while(cc.soldier_counter != 20)
    {
        l_rc = pthread_create(&input_t, NULL, &input, NULL);
        if(l_rc)
        {   
            printf("%d - %s\n", l_rc, strerror(l_rc));
            return 1;
        }
    }*/

    for(int i = 0 ; i < 5 ; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        l_rc = pthread_create(&SCV_T[i], NULL, &scv, (void *)id);

        if(l_rc)
        {   
            printf("%d - %s\n", l_rc, strerror(l_rc));
            return 1;
        }
    }

    for(int i = 0 ; i < 5 ; i++)
    {
        if(pthread_join(SCV_T[i], NULL) != 0)
        {
            perror("pthread_join() failed\n");
            return 1;
        }
    }

    /*if(pthread_join(input_t, NULL) != 0)
    {
        perror("pthread_join() failed\n");
        return 1;
    }*/

    printf("Map minerals %d, player minerals %d, SCVs %d, Marines %d\n", all_minerals, cc.all_the_scvs_mined, 5, cc.soldier_counter);
    
    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&soldier);
    free(blocks);
    free(flag_if_the_mineral_is_busy);
    free(size_of_blocks);
    
    return 0;
}
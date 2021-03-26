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

int minerals = 0;

int *blocks;
bool *flag_if_the_mineral_is_busy;
int size_of_blocks;
int all_minerals;

struct CommandCenter cc;

void *scv(void *args)
{
    int scv_counter;
    scv_counter = *((int*)args);

    for(int i = 0 ; i < size_of_blocks ; i++)
    {
        if(*(blocks + i) > 0)
        {
            //Придвижване до непразен минерален блок (симулира се време, 3s).
            sleep(3);

            //Проверка дали минералния блок е свободен (не се симулира допълнително време).
            while(*(blocks + i) > 0 && flag_if_the_mineral_is_busy[i] == 0)
            {
                int in_scv = 0;
                pthread_mutex_lock(&lock);

                //Копаене на минерали (не се симулира допълнително време).
                printf("SCV %d is mining from mineral block %d\n", scv_counter, i + 1);

                flag_if_the_mineral_is_busy[i] = 1;

                if(*(blocks + i) >= 8)
                {
                    in_scv = 8;
                    *(blocks + i) = *(blocks + i) - 8;
                }

                else if(*(blocks + i) < 8)
                {
                    in_scv = *(blocks + i);
                    *(blocks + i) = 0;
                }

                flag_if_the_mineral_is_busy[i] = 0;

                pthread_mutex_unlock(&lock);

                //Транспортиране на минералите до Командния център (симулира се време, 2s).
                
                printf("SCV %d is transporting minerals\n", scv_counter);
                sleep(2);

                pthread_mutex_lock(&lock);

                //Разтоварване на минералите (не се симулира допълнително време).
                printf("SCV %d delivered minerals to the Command center\n", scv_counter);
                cc.all_the_scvs_mined = cc.all_the_scvs_mined + in_scv;

                pthread_mutex_unlock(&lock);
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    cc.all_the_scvs_mined = 0;
    pthread_t *SCV_T = malloc(5 * sizeof(pthread_t));

    int l_rc;
    all_minerals = 0;

    pthread_mutex_init(&lock, NULL);

    if(argc < 2)
    {
        size_of_blocks = 2;

        blocks = malloc(size_of_blocks * sizeof(int));
        flag_if_the_mineral_is_busy = malloc(size_of_blocks * sizeof(bool));

        for(int i = 0 ; i < size_of_blocks ; i++)
        {
            blocks[i] = 500;
            all_minerals = all_minerals + blocks[i];
        }

        for(int i = 0 ; i < size_of_blocks ; i++)
        {
            flag_if_the_mineral_is_busy[i] = 0;
        }
    }

    else if(argc == 2)
    {
        size_of_blocks = atoi(argv[1]);

        blocks = malloc(size_of_blocks * sizeof(int));
        flag_if_the_mineral_is_busy = malloc(size_of_blocks * sizeof(bool));

        for(int i = 0 ; i < size_of_blocks ; i++)
        {
            blocks[i] = 500;
            all_minerals = all_minerals + 500;
        }

        for(int i = 0 ; i < size_of_blocks ; i++)
        {
            flag_if_the_mineral_is_busy[i] = 0;
        }
    }


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

    //Обучаването на войник струва 50 минерала и се симулира време за обучение 1s. 
    //След обучението на войник трябва да се изведе съобщението “You wanna piece of me, boy?”.
    while(cc.soldier_counter < 20)
    {
        char choice;
        choice = getc(stdin);

        if(choice == 'm')
        {
            if(cc.all_the_scvs_mined >= 50)
            {
                cc.all_the_scvs_mined = cc.all_the_scvs_mined - 50;
                sleep(1);
                cc.soldier_counter = cc.soldier_counter + 1;
                printf("You wanna piece of me, boy?\n");
            }

            else
            {
                printf("Not enough minerals.\n");
            }
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

    printf("Map minerals %d, player minerals %d, SCVs %d, Marines %d\n", all_minerals, cc.all_the_scvs_mined, 5, cc.soldier_counter);
    
    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);
    free(blocks);
    free(flag_if_the_mineral_is_busy);
    
    return 0;
}
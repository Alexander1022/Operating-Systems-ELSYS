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

#define SIZE 2

struct CommandCenter
{
    int all_the_scvs_mined;
    int soldier_counter;
};

pthread_mutex_t lock;
int minerals = 0;
int blocks[SIZE];
bool flag_if_the_mineral_is_busy[SIZE];

struct CommandCenter cc;

void *scv(void *args)
{
    int scv_counter;
    scv_counter = *((int*)args);

    pthread_mutex_lock(&lock);
    sleep(3);

    for(int i = 0 ; i < SIZE ; i++)
    {
        sleep(3);

        while(blocks[i] > 0)
        {
            sleep(3);
            if(blocks[i] > 0)
            {
                if(flag_if_the_mineral_is_busy[i] == 0)
                {
                    printf("SCV %d is mining from mineral block %d\n", scv_counter, i + 1);
                    flag_if_the_mineral_is_busy[i] = 1;
                    blocks[i] = blocks[i] - 8;
                    flag_if_the_mineral_is_busy[i] = 0;
                    printf("SCV %d is transporting minerals\n", scv_counter);
                    sleep(2);
                    printf("SCV %d delivered minerals to the Command center\n", scv_counter);
                    cc.all_the_scvs_mined = cc.all_the_scvs_mined + 8;

                    if(cc.soldier_counter != 20)
                    {
                        if(cc.all_the_scvs_mined > 50)
                        {
                            sleep(1);
                            cc.soldier_counter = cc.soldier_counter + 1;
                            cc.all_the_scvs_mined = cc.all_the_scvs_mined - 50; 
                            printf("You wanna piece of me, boy?\n");
                        }

                        else
                        {
                            printf("Not enough minerals.\n");
                        }
                    }
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
    
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    cc.all_the_scvs_mined = 0;
    pthread_t *SCV_T = malloc(5 * sizeof(pthread_t));

    int l_rc;

    int all_minerals = 0;

    if(argc < 2)
    {
        for(int i = 0 ; i < SIZE ; i++)
        {
            blocks[i] = 500;
            all_minerals = all_minerals + blocks[i];
        }

        for(int i = 0 ; i < SIZE ; i++)
        {
            flag_if_the_mineral_is_busy[i] = 0;
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

            if((5 + cc.soldier_counter) == 200)
            {
                return 1;
            }
        }

        for(int i = 0 ; i < 5 ; i++)
        {
            pthread_join(SCV_T[i], NULL);
        }
    }

    else if(argc == 2)
    {
        int size_of_blocks = atoi(argv[1]);

        for(int i = 0 ; i < size_of_blocks ; i++)
        {
            all_minerals = all_minerals + 500;
        }
    }


    printf("Map minerals %d, player minerals %d, SCVs %d, Marines %d\n", all_minerals, cc.all_the_scvs_mined, 5, cc.soldier_counter);

    return 0;
}
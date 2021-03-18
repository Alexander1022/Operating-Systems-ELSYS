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

pthread_mutex_t lock;
int minerals = 0; 
int blocks[2];

void *scv(void *args)
{
    int scv_counter;
    scv_counter = *((int*)args);
    pthread_mutex_lock(&lock);
    sleep(3);
    if(blocks[0] != 0)
    {
        printf("SCV %d is mining from mineral block 1\n", scv_counter);
    }

    else if(blocks[1] != 0)
    {
        sleep(3);
        printf("SCV %d is mining from mineral block 2\n", scv_counter);
    }

    printf("SCV %d is transporting minerals\n", scv_counter);
    
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main()
{
    pthread_t *SCV_T = malloc(5 * sizeof(pthread_t));

    int soldier_counter = 0;
    int l_rc;

    for(int i = 0 ; i < 2 ; i++)
    {
        blocks[i] = 500;
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

    for(int i = 0 ; i < 5 ; i++)
    {
        pthread_join(SCV_T[i], NULL);
    }

    soldier_counter++;

    return 0;
}
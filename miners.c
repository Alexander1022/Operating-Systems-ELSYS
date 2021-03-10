//--------------------------------------------
// NAME: Alexander Yordanov
// CLASS: XI b
// NUMBER: 1
// PROBLEM: #1 - threads
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

// създаван мютекса 
pthread_mutex_t lock;

// глобални променливи за златото, което имаме(gold) и общо колко сме изкопали (gold_mined)  
int gold;
int gold_mined;


//ф - ия за копач
void *miner(void *arg)
{
    for(int i = 0 ; i < 20 ; i++)
    {
        pthread_mutex_lock(&lock);
        gold = gold + 10;
        gold_mined = gold_mined + 10;
        printf("Miner %d gathered 10 gold\n", 1);
        sleep(2);
        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

// ф - ия за продавач
void *trader(void *arg)
{
    for(int i = 0 ; i < 20 ; i++)
    {
        pthread_mutex_lock(&lock);
        if(gold >= 10)
        {
            gold = gold - 10;
            printf("Trader %d sold 10 gold\n", 1);
            sleep(2);
        } 

        else
        {
            printf("The warehouse is empty, cannot sell!\n");
        }

        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t miner_t, trader_t;
    int l_rc;

    if(argc > 0)
    {
        int i = argv[0];
        int j = argv[1];

        printf("%d and %d\n", i, j);

        for(int z = 0 ; z < i ; z++)
        {
            if((l_rc = pthread_create(&miner_t, NULL, &miner, NULL)))
            {
                printf("%d - %s \n", l_rc, strerror(l_rc));
                return 1;
            }
        }


        for(int z = 0 ; z < j ; z++)
        {
            if((l_rc = pthread_create(&trader_t, NULL, &trader, NULL)))
            {
                printf("%d - %s \n", l_rc, strerror(l_rc));
                return 1;
            }
        }
    }

    else if(argc == 0) 
    {
        if((l_rc = pthread_create(&miner_t, NULL, &miner, NULL)))
        {
            printf("%d - %s \n", l_rc, strerror(l_rc));
            return 1;
        }

        if((l_rc = pthread_create(&trader_t, NULL, &trader, NULL)))
        {
            printf("%d - %s \n", l_rc, strerror(l_rc));
            return 1;
        }
    }
    

    pthread_join(miner_t, NULL);
    pthread_join(trader_t, NULL);

    pthread_mutex_destroy(&lock);

    printf("Gold: %d\n", gold);

    return 0;
}
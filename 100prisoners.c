#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


int drawers[100];
int prisoners[100];

pthread_mutex_t lock_global;
pthread_mutex_t lock_drawer[100];
void run_threads(int n,void* (*proc)(void*)){
    pthread_t t[n];
    for (int i = 0; i < 100/n; i++)
    {
        for (int j = 1; j < n+1; j++)
        {
            if ((j + i*n) > 100)
            {
                break;
            }
            pthread_create(&t[j-1],NULL,proc,(void*)(intptr_t)(j+i*n));
        }
        for (int j = 0; j < n; j++)
        {
            pthread_join(t[j],NULL);
        } 
    }  
}
static double timeit(int n, void* (*proc)(void *))
{   double result = 0;
    
    clock_t t1, t2;
    t1 = clock();
    run_threads(n, proc);
    t2 = clock();
    result += ((double) t2- (double) t1) / CLOCKS_PER_SEC * 1000; 
    
    return result;
}
void* random_global(void* number){
    pthread_mutex_lock(&lock_global);
    int id = (intptr_t)(number);
    int random = (rand() % 100);
    for (int i = 0; i < 50; i++)
    {
        if (id == drawers[random])
        {
            prisoners[id-1] = 1;
        }
        else{random = (rand()%100);}
    }  
    pthread_mutex_unlock(&lock_global);
    return NULL;
}
void* random_drawer(void* number){
    int id = (intptr_t)(number);
    int random = (rand() % 100);
    for (int i = 0; i < 50; i++)
    {   
        pthread_mutex_lock(&lock_drawer[random]);
        int random1 = random;
        
        if (id == drawers[random])
        {
            prisoners[id-1] = 1;
        }
        else{random = (rand() % 100);}
        pthread_mutex_unlock(&lock_drawer[random1]);
    }
    return NULL;    
}
void* smart_global(void* number){
    pthread_mutex_lock(&lock_global);
    int id = (intptr_t)(number);
    int random = id-1;
    for (int i = 0; i < 50; i++)
    {
        if (id == drawers[random])
        {
            prisoners[id-1] = 1;
        }
        else{random = drawers[random] - 1;}
    }  
    pthread_mutex_unlock(&lock_global);
    return NULL;
}
void* smart_drawer(void* number){
    int id = (intptr_t)(number);
    int random = id-1;
    for (int i = 0; i < 50; i++)
    {   
        pthread_mutex_lock(&lock_drawer[random]);
        int random1 = random;
        
        if (id == drawers[random])
        {
            prisoners[id-1] = 1;
        }
        else{random = drawers[random] - 1;}
        pthread_mutex_unlock(&lock_drawer[random1]);
    } 
    return NULL;   
}

int success(){
    for (int i = 0; i < 100; i++)
    {
        if (prisoners[i] == 0)
        {
            return 0;
        }     
    }
    return 1;
}
void put_random(int arr[]){
    int new_random;
    int boolean;
    for (int i = 0; i < 100; i++)
    {
        do
        {
            new_random = (rand() % 100) + 1;

            boolean = 1;
            for (int j = 0; j < i; j++)
            {
                if (arr[j] == new_random)
                {
                    boolean = 0;
                }
            } 
        } while (!boolean);
        arr[i] = new_random;
    } 
}


int main(int argc,char* argv[]){
    int num_of_games = 100;
    unsigned int seed_for_random = time(NULL);
    int win_random_global = 0,win_random_drawer = 0,win_smart_global=0,win_smart_drawer = 0;
    double time_random_global = 0,time_random_drawer = 0,time_smart_global=0,time_smart_drawer = 0;
    int opt;
    while ((opt = getopt(argc,argv,"n:s:"))!= -1)
    {
        switch (opt)
        {
        case 'n':
            num_of_games = atoi(optarg);
            break;
        case 's':
            seed_for_random = atoi(optarg);
        default:
            break;
        }
    }
    srand(seed_for_random);
    for (int i = 0; i < 100; i++)
    {
        if(pthread_mutex_init(&lock_drawer[i],NULL)!=0){
        fprintf(stderr,"Problem with lock\n");
        return 1;
    };
    }
    if(pthread_mutex_init(&lock_global,NULL)!=0){
        fprintf(stderr,"Problem with lock\n");
        return 1;
    };
    for (int i = 0; i < num_of_games; i++)
    {
        put_random(drawers);
        
        for (int i = 0; i < 100; i++) prisoners[i] = 0;
        time_random_global += timeit(100,&random_global);
        if(success() == 1){win_random_global++;}

        for (int i = 0; i < 100; i++) prisoners[i] = 0;
        time_random_drawer += timeit(100,&random_drawer);
        if(success() == 1){win_random_drawer++;}

        for (int i = 0; i < 100; i++) prisoners[i] = 0;
        time_smart_global += timeit(100,&smart_global);
        if(success() == 1){win_smart_global++;}

        for (int i = 0; i < 100; i++) prisoners[i] = 0;
        time_smart_drawer += timeit(100,&smart_drawer);
        if(success() == 1){win_smart_drawer++;}
    }
    printf("random_global       %d/%d wins/games = %.2lf'%%'  %.3lf/%d ms = %.3lf ms\n",win_random_global,num_of_games,((double)win_random_global/(double)num_of_games)*100,time_random_global,num_of_games,time_random_global/num_of_games);
    printf("random_drawer       %d/%d wins/games = %.2lf'%%'  %.3lf/%d ms = %.3lf ms\n",win_random_drawer,num_of_games,((double)win_random_drawer/(double)num_of_games)*100,time_random_drawer,num_of_games,time_random_drawer/num_of_games);
    printf("smart_global        %d/%d wins/games = %.2lf'%%'  %.3lf/%d ms = %.3lf ms\n",win_smart_global,num_of_games,((double)win_smart_global/(double)num_of_games)*100,time_smart_global,num_of_games,time_smart_global/num_of_games);
    printf("smart_drawer        %d/%d wins/games = %.2lf'%%'  %.3lf/%d ms = %.3lf ms\n",win_smart_drawer,num_of_games,((double)win_smart_drawer/(double)num_of_games)*100,time_smart_drawer,num_of_games,time_smart_drawer/num_of_games);
    return 0;   
}
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>
void *inc(void *arg);
void *dec(void *arg);
long long sum = 0;
pthread_mutex_t mutex;
int main(int argc, char *argv[]){
    pthread_t threads[100];
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 100; i++){
        if(i % 2)
            pthread_create(&threads[i], NULL, inc, NULL);
        else
            pthread_create(&threads[i], NULL, dec, NULL);
    }
    for(int i = 0; i < 100; i++)
        pthread_join(threads[i], NULL);
    pthread_mutex_destroy(&mutex);
    printf("Result: %lld\n", sum);
    puts("end of main");
    return 0;
}

void *inc(void *arg){
    for(int i = 0; i < 50000; i++){
        pthread_mutex_lock(&mutex);
        sum += 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *dec(void *arg){
    for(int i = 0; i < 50000; i++){
        pthread_mutex_lock(&mutex);
        sum -= 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

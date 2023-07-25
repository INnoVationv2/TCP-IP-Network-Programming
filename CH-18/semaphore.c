#include<stdio.h>
#include<pthread.h>
#include <semaphore.h>
void *read(void *arg);
void *write(void *arg);
int num = 0;
sem_t sem_one, sem_two;
int main(int argc, char *argv[]){
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    pthread_t p_read, p_write;
    pthread_create(&p_read, NULL, read, NULL);
    pthread_create(&p_write, NULL, write, NULL);

    pthread_join(p_read, NULL);
    pthread_join(p_write, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    puts("end of main");
    return 0;
}

void* read(void* args){
    for(int i = 0; i < 5; i++){
        fputs("Input num: ", stdout);
        sem_wait(&sem_two);
        scanf("%d", &num);
        sem_post(&sem_one);
    }
    return NULL;
}

void* write(void* args){
    int sum = 0;
    for(int i = 0; i < 5; i++){
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    printf("Result: %d\n", sum);
    return NULL;
}



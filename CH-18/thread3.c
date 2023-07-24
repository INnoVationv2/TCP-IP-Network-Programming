#include<stdio.h>
#include<pthread.h>
#include <unistd.h>
int sum = 0;
void *inc(void *arg);
void *dec(void *arg);

int main(int argc, char *argv[]){
    pthread_t inc_id, dec_id;
    if(pthread_create(&inc_id, NULL, inc, NULL) != 0){
        puts("pthread create() error");
        return -1;
    }
    if(pthread_create(&dec_id, NULL, dec, NULL) != 0){
        puts("pthread create() error");
        return -1;
    }

    pthread_join(inc_id, NULL);
    pthread_join(dec_id, NULL);
    printf("Result: %d\n", sum);
    puts("end of main");
    return 0;
}

void *inc(void *arg){
    for(int i = 0; i < 50000; i++)
        sum += 1;
    return NULL;
}

void *dec(void *arg){
    for(int i = 0; i < 50000; i++)
        sum -= 1;
    return NULL;
}

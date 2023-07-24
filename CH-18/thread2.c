#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

void *thread_main(void *arg);

int main(int argc, char *argv[]){
    pthread_t t_id;
    int thread_param = 5;
    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0){
        puts("pthread create() error");
        return -1;
    }

    void *status;
    pthread_join(t_id, &status);
    printf("%s\n", (char*)status);
    puts("end of main");
    return 0;
}

void *thread_main(void *arg){
    int i;
    int cnt = *((int*)arg);
    for(i = 0; i < cnt; i++){
        sleep(1);
        puts("running thread");
    }
    char *msg = "Hello, I'am thread";
    return (void*)msg;
}
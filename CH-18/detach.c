#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

void *test(void *arg);
int main(int argc, char *argv[]){
    pthread_t thread;
    pthread_create(&thread, NULL, test, NULL);
    pthread_detach(thread);
//    sleep(10);
    puts("end of main");
    return 0;
}

void *test(void *arg){
    for(int i = 0; i < 5; i++){
        sleep(1);
        printf("inc--%d\n", i);
    }
    return NULL;
}


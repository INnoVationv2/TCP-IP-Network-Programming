#include <cstdio>
#include <unistd.h>
#include <csignal>

void timeout(int sig){
//    if(sig == SIGALRM)
        printf("Time Out\n");
    alarm(2);
}

void exit(int sig){
//    if(sig == SIGINT)
        printf("CTRL+C pressed.\n");
}

int main(){
    struct sigaction handler{};
    handler.sa_handler = timeout;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGALRM, &handler, nullptr);
//    signal(SIGALRM, timeout);
//    signal(SIGINT, exit);
    alarm(2);
    for(int i = 0; i < 3; i++)
    {
        printf("Wait...\n");
        sleep(100);
    }
}
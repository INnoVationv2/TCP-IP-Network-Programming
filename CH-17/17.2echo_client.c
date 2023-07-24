#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

void error_handling(char* msg);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr, client_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =  inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    char msg[] = "Hello";
    write(sock, msg, strlen(msg));
    memset(&msg, 0, strlen(msg));
    ssize_t sz = 0;
    while(sz < strlen("Hello")){
        ssize_t rd = read(sock, msg, sizeof(msg));
        sz += rd;
    }
    printf("Msg from server: %s\n", msg);
    close(sock);
}

void error_handling(char *msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
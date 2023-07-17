#include <cstdio>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

using std::string;
using std::cin;

void error_handling(const string&);
void echo(int);

//多线程处理echo请求
int main(int argc, char *argv[]){
    int server_sock;
    int client_sock;

    struct sockaddr_in server_addr{};
    struct sockaddr_in client_addr{};
    socklen_t client_addr_size;

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        error_handling("bind() error");

    if(listen(server_sock, 1) == -1)
        error_handling("listen() error");

    client_addr_size = sizeof client_addr;

    while(true){
        printf("Waiting new request.\n");
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
        printf("Receive new request, Start process.\n");
        if(client_sock == -1)
            error_handling("accept() error");
        if(fork() == 0)
            echo(client_sock);
    }

    close(server_sock);
    return 0;
}

void echo(int client_sock){
    printf("--Create new process %d to handle.\n", getpid());
    char msg[30];
    ssize_t strlen = read(client_sock, msg, sizeof msg);
    if(strlen == -1)
        error_handling("read() error");
    printf("--Msg from client: %s\n", msg);
    printf("--Send back\n");
    write(client_sock, msg, strlen);
    close(client_sock);
    exit(1);
}

void error_handling(const string& msg)
{
    fputs(msg.c_str(), stderr);
    fputc('\n', stderr);
    exit(1);
}
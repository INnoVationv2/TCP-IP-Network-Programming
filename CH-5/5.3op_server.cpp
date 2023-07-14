#include <cstdio>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <string>

using std::string;
using std::cin;
using std::to_string;

#define INT_SIZE sizeof(int)

void error_handling(const string&);
void calc(int);

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
            calc(client_sock);

        exit(1);
    }
}

void calc(int client_sock){
    int cnt;
    char op;
    int vals[100];
    printf("--Start new process %d to handle.\n", getpid());
    read(client_sock, &cnt, INT_SIZE);
    for(int i = 0; i < cnt; i++){
        read(client_sock, &vals[i], INT_SIZE);
    }
    read(client_sock, &op, sizeof(char));
    int res = vals[0];
    switch (op) {
        case '+':
            for(int i = 1; i < cnt; i++)
                res += vals[i];
            break;
        case '-':
            for(int i = 1; i < cnt; i++)
                res -= vals[i];
            break;
        case '*':
            for(int i = 1; i < cnt; i++)
                res *= vals[i];
            break;
        case '/':
            for(int i = 1; i < cnt; i++)
                res /= vals[i];
            break;
    }
    write(client_sock, to_string(res).c_str(), sizeof(int));
    close(client_sock);
    exit(1);
}

void error_handling(const string& msg)
{
    fputs(msg.c_str(), stderr);
    fputc('\n', stderr);
    exit(1);
}
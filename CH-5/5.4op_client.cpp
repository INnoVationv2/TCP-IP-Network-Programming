#include <cstdio>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

using std::string;
using std::to_string;
#define INT_SIZE sizeof(int)

void error_handling(const string& msg);
void calc(int);

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in server_addr{};


    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    printf("Connected...\n");
    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        error_handling("connect() error");

    close(sock);
    return 0;
}

void calc(int sock){
    int cnt;
    char msg[102400];
    printf("Operand count: ");
    scanf("%d", (int*)&msg[0]);
    cnt = *(int*)&msg[0];

    for(int i = 1; i <= cnt; i++){
        printf("Operand %d: ", i);
        scanf("%d", (int*)&msg[i * INT_SIZE]);
    }

    printf("Operator: ");
    scanf("%s", &msg[INT_SIZE * (cnt + 1)]);
    ssize_t str_len = write(sock, msg, INT_SIZE * (cnt + 1) + 1);
    if(str_len == -1)
        error_handling("write() error");

    read(sock, msg, sizeof(uint32_t));
    printf("Msg from server: %s\n", msg);
}

void error_handling(const string& msg){
    fputs(msg.c_str(), stderr);
    fputc('\n', stderr);
    exit(1);
}
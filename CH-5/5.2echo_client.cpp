#include <cstdio>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

using std::string;

void error_handling(const string& msg);

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in server_addr{};
    char msg[30] = "hello!";
    ssize_t str_len;

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

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        error_handling("connect() error");

    printf("Send msg to server: %s\n", msg);
    str_len = write(sock, msg, sizeof (msg) - 1);
    if(str_len == -1)
        error_handling("write() error");

    str_len = read(sock, msg, sizeof(msg) - 1);
    if(str_len == -1)
        error_handling("read() error");

    printf("Msg from server: %s.\n", msg);
    close(sock);
    return 0;
}

void error_handling(const string& msg)
{
    fputs(msg.c_str(), stderr);
    fputc('\n', stderr);
    exit(1);
}
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

//Echo Client, based on UDP
int main(int argc, char *argv[]){
    int sock;
    char msg[100] = "hello, world!";
    struct sockaddr_in server_addr{};
    struct sockaddr_in client_addr{};
    socklen_t client_addr_size;


    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    sendto(sock, msg, strlen(msg), 0,
           (struct sockaddr*)&server_addr, sizeof(server_addr));
    recvfrom(sock, msg, 100, 0,
             (struct sockaddr*)&client_addr, &client_addr_size);
    printf("Receive From Server: %s\n", msg);
    close(sock);
}


void error_handling(const string& msg)
{
    fputs(msg.c_str(), stderr);
    fputc('\n', stderr);
    exit(1);
}
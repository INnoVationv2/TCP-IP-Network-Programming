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

//Echo server, based on UDP
int main(int argc, char *argv[]){
    int server_sock;
    char msg[100];
    struct sockaddr_in server_addr{};
    struct sockaddr_in client_addr{};
    socklen_t client_addr_size;


    server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(server_sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        error_handling("bind() error");

    for(int i = 0; i < 5; i++){
        client_addr_size = sizeof(client_addr);
        recvfrom(server_sock, msg, 100, 0,
                 (struct sockaddr*)&client_addr, &client_addr_size);
        printf("Receive from client: %s\n", msg);
        sendto(server_sock, msg, strlen(msg), 0,
               (struct sockaddr*)&client_addr, sizeof(client_addr));
        printf("Send Back.\n");
    }
    close(server_sock);
}


void error_handling(const string& msg)
{
    fputs(msg.c_str(), stderr);
    fputc('\n', stderr);
    exit(1);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char*);

int main(int argc, char* argv[]){
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error.\n");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error.\n");

    fd_set read_fd, cp_read_fd;
    FD_ZERO(&read_fd);
    FD_SET(serv_sock, &read_fd);
    int fd_max = serv_sock;
    char buf[BUF_SIZE];

    while(1){
        cp_read_fd = read_fd;
        struct timeval time_out;
        time_out.tv_sec = 5;
        int res;
        if((res = select(fd_max + 1, &cp_read_fd, 0, 0, &time_out)) == -1)
            break;
        if(res == 0)
            continue;
        for(int i = 0; i <= fd_max; i ++ ){
            if(FD_ISSET(i, &cp_read_fd)){
                if(i == serv_sock){
                    socklen_t size = sizeof(client_addr);
                    client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &size);
                    if(fd_max < client_sock)
                        fd_max = client_sock;
                    printf("Connected client: %d.\n", client_sock);
                } else {
                    ssize_t strlen = read(i, buf, BUF_SIZE);
                    if(strlen == 0){
                        FD_CLR(i, &read_fd);
                        close(i);
                        printf("closed client: %d.\n", i);
                    } else {
                        write(i, buf, strlen);
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
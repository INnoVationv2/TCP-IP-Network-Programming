#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

/*
 * Test Level-Triggered
 * 测试条件触发
*/
void error_handling(char* msg);
#define EPOLL_SIZE 50
int main(int argc, char* argv[]){
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bing() error.");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error.");

    int epfd = epoll_create(EPOLL_SIZE);
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    struct epoll_event* ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    int times = 0;
    while(1){
        int event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }
        for(int i = 0; i < event_cnt; i++){
            if(ep_events[i].data.fd == serv_sock){
                socklen_t addr_sz = sizeof(client_addr);
                client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &addr_sz);
                event.events = EPOLLIN;
                event.data.fd = client_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);
                printf("Connected client %d\n", client_sock);
            } else {
                char buf[30];
                //read 1 Byte each time
                ssize_t strlen = read(ep_events[i].data.fd, buf, 1);
                if(strlen == 0){
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("Closed Clients: %d\n", ep_events[i].data.fd);
                } else {
                    printf("Socket:%d, %d time read\n", ep_events[i].data.fd, times ++);
                    //write(ep_events[i].data.fd, buf, strlen);
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void error_handling(char *msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
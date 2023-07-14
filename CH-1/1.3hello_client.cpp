#include <cstdio>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
//#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

using std::string;

void error_handling(const string& msg);

union in_addr_test{
    struct{uint8_t fir:8, sec:8, thi: 8, four: 8;} ip_split;
    in_addr_t s_addr;
};

struct sockaddr_in_test {
    __uint8_t       sin_len;
    sa_family_t     sin_family;
    in_port_t       sin_port;
    in_addr_test    sin_addr;
    char            sin_zero[8];
};

// 验证TCP不存在数据边界，即write和read次数不用对应
int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in server_addr{};
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.1");
    char msg[30];
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

    // 每次只读取1B
    int idx = 0;
    while((str_len= read(sock, &msg[idx++], 1))){
        if(str_len == -1)
            error_handling("read() error");
    }

    printf("Msg from server: %s.\n", msg);
    printf("Read is been called %d times.\n", idx);
    close(sock);
    return 0;
}

void error_handling(const string& msg)
{
    fputs(msg.c_str(), stderr);
    fputc('\n', stderr);
    exit(1);
}
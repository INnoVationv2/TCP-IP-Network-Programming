#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void *arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char* msg);

int main(int argc, char* argv[]){
    int serv_sock, client_sock;
    struct sockaddr_in serv_adr, client_adr;
    int client_adr_size;
    char buf[BUF_SIZE];
    pthread_t t_id;
    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock, 20) == -1)
        error_handling("listen() error");

//    while(1){
        client_adr_size = sizeof(client_adr);
        client_sock = accept(serv_sock, (struct sockaddr*)&client_adr, &client_adr_size);
        printf("Connection request: %s:%d\n", inet_ntoa(client_adr.sin_addr), ntohs(client_adr.sin_port));
        request_handler(&client_sock);
//        pthread_create(&t_id, NULL, request_handler, &client_sock);
//        pthread_detach(t_id);
//    }
    close(serv_sock);
    return 0;
}

void* request_handler(void *arg){
    int client_sock = *((int*)arg);
    char req_line[SMALL_BUF];
    FILE *client_read, *client_write;
    char method[10];
    char ct[15];
    char file_name[30];

    client_read = fdopen(client_sock, "r");
    client_write = fdopen(dup(client_sock), "w");

    fgets(req_line, SMALL_BUF, client_read);
    if(strstr(req_line, "HTTP/") == NULL){
        send_error(client_write);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if(strcmp(method, "GET") != 0){
        send_error(client_write);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }
    fclose(client_read);
    send_data(client_write, ct, file_name);
    return NULL;
}

void send_data(FILE *fp, char *ct, char *file_name){
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE* send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");
    if(send_file == NULL){
        send_error(fp);
        return;
    }

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    while(fgets(buf, BUF_SIZE, send_file) != NULL){
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

char* content_type(char *file){
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if(!strcmp(extension, "html") || !strcmp(extension, "htm"))
        return "text/html";
    else
        return "text/plain";
}

void send_error(FILE *fp){
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size = 5><br> ERROR </font></body>"
                     "</html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

void error_handling(char *msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}


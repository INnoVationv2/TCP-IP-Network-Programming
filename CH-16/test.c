#include<stdio.h>
#include <unistd.h>
#include <string.h>
#define INT_SIZE sizeof(int)

int main(){
    char buf[30];
    *(int*)buf = 65535;
    scanf("%d", buf);
    int val = buf[0];
    memcpy(&val, buf, 4);
    printf("%d\n", *buf);
}
//18748507626
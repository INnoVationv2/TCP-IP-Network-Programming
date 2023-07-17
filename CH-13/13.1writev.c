#include <stdio.h>
#include <sys/uio.h>
#include <string.h>

int main(){
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "123456";
    vec[0].iov_base = buf1;
    vec[0].iov_len = strlen(buf1);
    vec[1].iov_base = buf2;
    vec[1].iov_len = strlen(buf2);

    ssize_t strlen = writev(fileno(stdout), vec, 2);
    puts("");
    printf("Write Bytes: %zd.\n", strlen);


    strlen = readv(fileno(stdin), vec, 2);
    printf("%s, %s", buf1, buf2);
    puts("");
    printf("Read Bytes: %zd.\n", strlen);
    return 0;
}
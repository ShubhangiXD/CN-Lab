#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

int main()
{
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket error\n");
        exit(1);
    }
    else
    printf("socket created successfully\n");
    return 0;
}
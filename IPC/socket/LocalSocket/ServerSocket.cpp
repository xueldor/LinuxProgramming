#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<errno.h>
#include<stddef.h>
#include<unistd.h>

#define MAX_CONNECT_NUM 2
#define BUFFER_SIZE 1024
#define SOCKET_PATH "test_uds.tmp"

static int fd;

int requestConnectService(){
    struct sockaddr_un server_sockaddr;
    fd= socket(AF_UNIX,SOCK_SEQPACKET,0);
    if(fd < 0) {
        perror("socket error");
        return -1;
    }
    unlink(SOCKET_PATH);
    server_sockaddr.sun_family=AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCKET_PATH);

    if(bind(fd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr)) <0 ){
        perror("bind failed");
        close(fd);
        fd = -1;
        return -1;
    }
    if(listen(fd,MAX_CONNECT_NUM) < 0){
        perror("listen failed");
        close(fd);
        fd = -1;
        return -1;
    }
    return fd;
}

int main(){
    requestConnectService();
    struct sockaddr_un client_addr;
    char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);

    int cli_fd = accept(fd, NULL, NULL);
    if(cli_fd < 0){
        perror("accept error");
    }

    while(1){
        int ret = recv(cli_fd, buffer, BUFFER_SIZE, 0);
        if(ret < 0){
            printf("recv failed\n");
            break;
        }
        printf("recv client: %s",buffer);
        send(cli_fd, buffer, ret, 0);
    }
    printf("close socket client");
    close(cli_fd);
    close(fd);
    unlink(SOCKET_PATH);//删除
}
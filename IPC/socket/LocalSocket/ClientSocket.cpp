
#include <cstdio>
#include "ClientSocket.h"
ClientSocket::ClientSocket():fd(-1){
}
ClientSocket::~ClientSocket(){
}

int ClientSocket::requestConnectService(){
    int result=-1;
    struct sockaddr_un server_sockaddr;
    fd= socket(AF_UNIX,SOCK_SEQPACKET,0);
    if(fd < 0) {
        perror("socket error");
        return -1;
    }
    server_sockaddr.sun_family=AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCKET_PATH);
    socklen_t len_t=sizeof(server_sockaddr);
    result=connect(fd,( struct sockaddr * )&server_sockaddr,len_t);//第三个参数是第二个参数addr的大小
    if(result<0){
        close(fd);
        return -1;
    }
    return fd;
}

int ClientSocket::readSocket(char *buf, unsigned int size) {
    int ret = -1;
    ret = read(fd,buf,size);
    if(ret<0){
        return -1;
    }
    return ret;
}

int ClientSocket::writeSocket(char* buf,unsigned int size){
    int ret = -1;
    ret = write(fd,buf,size);
    if(ret<0){
        return -1;
    }
    return ret;
}
void ClientSocket::closeConn() {
    close(fd);
}

int main(){
    ClientSocket clientSocket;
    clientSocket.requestConnectService();
    int num=0;
    while (1) {
        num++;
        char buff[1024];
        sprintf(buff, "hello world,%d...\n", num);
        printf("client send: %s", buff);
        int ret = clientSocket.writeSocket(buff, strlen(buff) + 1);
        if(ret <= 0){
            printf("end");
            break;
        }
        sleep(1);
    }
    clientSocket.closeConn();

}

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(){
    //1. 创建通讯的套接字，命名为fd
    printf("1.create socket for communication\n");
    int fd = socket(AF_INET, SOCK_STREAM, 0);//参数说明：1.ipv4类型 2.tcp->socket流 stream 3.default 0
    if(fd==-1){
        perror("socket error");
        return -1;
    }

    //2.连接服务器的ip地址和端口号
    printf("2.connect server ip-port\n");
    struct sockaddr_in saddr = {
            .sin_family = AF_INET,//与套接字的ipv4类型一致
            .sin_port = htons(9999),//服务器监听的端口号，注意要从主机字节序转为网络子字节序
    };
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);//"127.0.0.1"转成网络字节序列
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("connect error");
        return -1;
    }

    //3.与服务端通讯
    printf("3.communicate with client\n");
    int num=0;
    while (1){
        num++;
        //发送数据
        char buff[1024];
        sprintf(buff, "hello world,%d...\n", num);
        send(fd,buff,strlen(buff)+1,0);//length=char-length+"\0"
        //接收数据
        int len = recv(fd, buff, sizeof(buff), 0);
        if(len > 0){
            printf("server say:%s\n", buff);
        }else if(len == 0){
            printf("server disconnect\n");
            break;
        } else{
            perror("receive error");
            break;
        }
        sleep(1);
    }
    close(fd);
    return 0;
}
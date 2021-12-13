
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/**
 * 开两个shell窗口，一个运行server，一个运行client
 */
int main(){
    //1.创建负责监听的套接字，命名为fd
    printf("1.create socket for listening\n");
    int fd = socket(AF_INET, SOCK_STREAM, 0);//参数说明：1.ipv4类型 2.tcp->socket流 stream 3.default 0
    if(fd==-1){
        perror("socket error");
        return -1;
    }

    //2.套接字绑定本地的ip地址和端口号
    printf("2.binding local ip-port\n");
    struct sockaddr_in saddr = {
            .sin_family = AF_INET,//与套接字的ipv4类型一致
            .sin_port = htons(9999),//监听的本地端口号（注意高低字节序转换,网络传输数据是大端模式，本地整形可能是小端模式，故一定要记得转, htonl即"host to network long",类似的htons表示“host to network short”,后面的ntohs反之）
            .sin_addr.s_addr = htonl(INADDR_ANY)//INADDR_ANY = 0,即表示地址为0.0.0.0,这个地址会自动对应到本地ip地址
//            .sin_addr = inet_addr("127.0.0.1")
    };
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("binding error");
        close(fd);
        return -1;
    }

    //3.设置监听
    printf("3.set listen\n");
    ret = listen(fd, 10);
    if(ret == -1){
        perror("listen error");
        close(fd);
        return -1;
    }

    //4. 阻塞并等待连接
    printf("4. block and waiting for connection\n");
    struct sockaddr_in caddr;
    socklen_t addlen = sizeof(caddr);
    int cfd = accept(fd, (struct sockaddr*) &caddr, &addlen);//参数说明1.负责监听的套接字 2.接收到的客户端地址，因此创建了一个与客户端连接之后用于通讯的套接字cfd
    if(cfd == -1){
        perror("accept error\n");
        return -1;
    }

    //5.连接成功，打印客户端信息并进行通讯
    printf("5.connect successfully,print related infomation,then communicate with client\n");
    char ip[32];
    //inet_pton: pton应该是p_to_n, p代表表达（presentation),n代表数值（numeric)
    //将点分十进制的ip地址转化为用于网络传输的数值格式,对于IPv4地址和IPv6地址都适用
    //"点分十进制的ip地址"就是“192.168.12.25”这样形式的字符串，是一个char数组
    //"网络传输的数值格式"是in6_addr或in_addr结构体，实质是int，代表网络上传输的二进制
    //inet_ntop反之
    inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip));
    printf("client ip:%s,port:%d\n", ip, ntohs(caddr.sin_port));//ntohs()将一个16位数由网络字节顺序转换为主机字节顺序

    while (1){
        //接收数据
        char buff[1024];
        int len = recv(cfd, buff, sizeof(buff), 0);
        if(len > 0){
            printf("client say:%s\n", buff);
            send(cfd, buff, len, 0);
        }else if(len == 0){
            printf("client disconnect\n");
            break;
        } else{
            perror("receive error");
            break;
        }
    }
    close(fd);
    close(cfd);
    return 0;
}
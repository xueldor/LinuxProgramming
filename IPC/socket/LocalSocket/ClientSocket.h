#ifndef INET_SOCKET_CLIENTSOCKET_H
#define INET_SOCKET_CLIENTSOCKET_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "test_uds.tmp"

class ClientSocket{
public:
    ClientSocket();
    virtual ~ClientSocket();
    int requestConnectService();
    int readSocket(char* buf,unsigned int size);
    int writeSocket(char* buf,unsigned int size);
    void closeConn();
private:
    int fd;
};

#endif //INET_SOCKET_CLIENTSOCKET_H

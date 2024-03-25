#ifndef __SERVER_H__
#define  __SERVER_H__

#include <iostream>
#include <thread>
#include "tcpsocket.h"
#include <QByteArray>
#include <QDebug>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <logData.h>

using namespace std;
/*服务器*/
typedef struct client_node {
    SOCKET sock;
    char name[64];
    struct client_node** prev;//shao zuo yi bi pan duan
    struct client_node* next;
} client_node_t;


void add_client(client_node_t* p);
void remove_client(client_node_t* p);

void child_func(SOCKET clifd);

void child_connect(SOCKET clifd, SOCKET serfd);



class Server : public QObject{
    Q_OBJECT
public:
    Server(const char* ip, unsigned short PORT);
    ~Server();

private:
    //服务器
    SOCKET serfd;
    //客户端
    SOCKET clifd;
public:

    friend void child_func(SOCKET clifd);
    friend void child_connect(SOCKET clifd, SOCKET serfd);

};



#endif

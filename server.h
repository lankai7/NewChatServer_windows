#ifndef __SERVER_H__
#define  __SERVER_H__

#include <iostream>
#include <thread>
#include "tcpSocket.h"
#include "fileOperation.h"
using namespace std;
/*服务器*/

typedef struct client_node {
    SOCKET sock;
    char name[64];
    struct client_node** prev;//shao zuo yi bi pan duan
    struct client_node* next;
} client_node_t;

client_node_t* g_client = NULL;

void add_client(client_node_t* p) {
    p->next = g_client;
    p->prev = &g_client;
    g_client = p;
}
void remove_client(client_node_t* p) {
    *(p->prev) = p->next;
    if (p->next) {
        p->next->prev = p->prev;
    }
}

void child_func(SOCKET clifd) {
    int ret;
    char recvBuf[1024] = "";
    client_node_t* cur;//dang qian de client
    //可以与客户端进行通信了
    memset(recvBuf, 0, sizeof(recvBuf));

    ret = recv(clifd, recvBuf, sizeof(recvBuf), 0);
    //xian shou yi ge yong hu ming
    if (ret == 0) {
        cout << "客户端ddd正常下线..." << endl;
    }
    else if (ret < 0) {
        err("recv");
    }
    cur = (client_node_t*)malloc(sizeof(client_node_t));
    cur->sock = clifd;
    strcpy(cur->name, recvBuf);
    add_client(cur);
    while (1) {
        memset(recvBuf, 0, sizeof(recvBuf));
        ret = recv(clifd, recvBuf, sizeof(recvBuf), 0);
        if (ret == 0) {
            cout << "客户端正常下线..." << endl;
            break;
        }
        else if (ret < 0) {
            err("recv");
            break;
        }
        cout << "received:" << recvBuf << endl;
        client_node_t* p = g_client;
        while (p) {
            if (p != cur) {
                char msg[1024];
                sprintf(msg, "%s:%s", cur->name, recvBuf);
                send(p->sock, msg, sizeof(msg), 0);
            }
            p = p->next;
        }
        if (strcmp(recvBuf, "over") == 0)
            break;
    }
    remove_client(cur);
    free(cur);
    closesocket(clifd);
}

void child_connect(SOCKET clifd, SOCKET serfd) {
    while (1) {
        clifd = accept(serfd, NULL, NULL);
        if (clifd == INVALID_SOCKET) {
            thread proc0(child_connect, clifd, serfd);
            proc0.detach();
            break;
        }
        cout << "连接成功..." << endl;
        thread proc(child_func, clifd);
        proc.detach();
    }
}

class Server {
public:
    Server() {
        init_Socket();

        serfd = create_serverSocket();
        cout << "server create successed ,wait for client connet..." << endl;

        //等待客户端连接
        thread proc0(child_connect, clifd, serfd);
        proc0.detach();

    }
    ~Server() {
        closesocket(serfd);
        closesocket(clifd);
        close_Socket();
    }

private:
    //服务器
    SOCKET serfd;
    //客户端
    SOCKET clifd;
};



#endif

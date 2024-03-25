#ifndef __SERVER_H__
#define  __SERVER_H__

#include <iostream>
#include <thread>
#include "tcpSocket.h"
#include <QByteArray>
#include <QDebug>


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
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
    char recvName[256] = "";
    char recvType[256] = "";
    char recvData[102400] = "";

    long g_num;
    char* g_data;
    client_node_t* cur;//dang qian de client
    //可以与客户端进行通信了
    memset(recvName, 0, sizeof(recvName));

    ret = recv(clifd, recvName, sizeof(recvName), 0);
    //xian shou yi ge yong hu ming
    if (ret == 0) {
        qDebug() << "client off..." << endl;
    }
    else if (ret < 0) {
        err("recv");
    }
    cur = (client_node_t*)malloc(sizeof(client_node_t));
    cur->sock = clifd;
    strcpy(cur->name, recvName);
    add_client(cur);
    while (1) {
        memset(recvType, 0, sizeof(recvType));
        memset(recvData, 0, sizeof(recvData));
        int retType = recv(clifd, recvType, sizeof(recvType), 0);
        int retData = recv(clifd, recvData, sizeof(recvData), 0);

        if(strcmp(recvType, "type_pic_") == 0){
            char* endptr;
            g_num = std::strtol(recvData, &endptr, 10);
            g_data = new char[g_num];
            int ret = recv(clifd, g_data, g_num, 0);
            if (ret == 0) {
                qDebug() << "服务器正常" << endl;
            }
            else if (ret < 0) {
                err("recv");
            }

        }

        if (retType <= 0||retData <= 0) {
            qDebug() << "client off..." << endl;
            break;
        }

        qDebug() << recvType <<":" << recvData << endl;
        client_node_t* p = g_client;
        while (p) {
            if (p != cur) {
                //char msg[1024];
                //sprintf(msg, "%s:%s", cur->name, recvBuf);    //合并名字与文本（弃用）
                send(p->sock, cur->name, sizeof(cur->name), 0);
                send(p->sock, recvType, sizeof(recvType), 0);
                if(strcmp(recvType, "type_pic_") == 0){
                    std::string str = std::to_string(g_num);
                    const char* buffer = str.c_str();
                    retData = send(p->sock, buffer, sizeof(buffer), 0);

                    int ret = send(p->sock, g_data, g_num, 0);
                    if (ret == SOCKET_ERROR) {
                        err("sendFile");
                    }
                    delete[] g_data;
                    qDebug() << "发送成功：" << g_num << "Byte" << endl;
                }
                else if(strcmp(recvType, "type_msg_") == 0||strcmp(recvType, "type_sys_") == 0){
                    int ret0 = send(p->sock, recvData, sizeof(recvData), 0);
                    if (ret0 == SOCKET_ERROR) {
                        err("sendFile");
                    }
                }

            }
            p = p->next;
        }
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
        qDebug() << "connect successed..." << endl;
        thread proc(child_func, clifd);
        proc.detach();
    }
}



class Server {
public:
    Server(const char* ip, unsigned short PORT) {
        init_Socket();

        serfd = create_serverSocket(ip,PORT);
        qDebug() << "server create successed ,wait for client connet..." << endl;

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

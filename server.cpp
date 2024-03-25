#include "Server.h"

client_node_t* g_client = NULL;
logData* log = new logData;

void add_client(client_node_t *p)
{
    p->next = g_client;
    p->prev = &g_client;
    g_client = p;
}

void remove_client(client_node_t *p)
{
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
        QString time = log->getTime();
        emit log->logPut("[" + time + "]" + QString(recvName) + "客户端断开...\n");
    }
    else if (ret < 0) {
        err("recv");
    }
    cur = (client_node_t*)malloc(sizeof(client_node_t));
    cur->sock = clifd;
    strcpy(cur->name, recvName);
    QString time = log->getTime();
    emit log->logPut("[" + time + "]" + QString(recvName) + "已连接成功\n");
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
            QString time = log->getTime();
            emit log->logPut("[" + time + "]" + QString(recvName) + "已断开连接\n");
            break;
        }

        qDebug() << recvName <<"["<< recvType <<"]"<<":" << recvData << endl;
        QString time = log->getTime();
        emit log->logPut("[" + time + "]" + QString(recvName) + "(" + QString(recvType) + ")" + ":" + QString(recvData)+"\n");
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
                    QString time = log->getTime();
                    emit log->logPut("[" + time + "]" + "文件数据大小："+QString::number(g_num)+" Byte\n");
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
        QString time = log->getTime();
        emit log->logPut("[" + time + "]" + "通讯连接中...\n");
        thread proc(child_func, clifd);
        proc.detach();
    }
}

Server::Server(const char *ip, unsigned short PORT) {
    init_Socket();

    serfd = create_serverSocket(ip,PORT);
    qDebug() << "server create successed ,wait for client connet..." << endl;
    QString time = log->getTime();
    emit log->logPut("[" + time + "]" + "服务器开启成功，等待客户端连接...\n");
    //等待客户端连接
    std::thread proc0(child_connect, clifd, serfd);
    proc0.detach();

}

Server::~Server() {
    closesocket(serfd);
    closesocket(clifd);
    close_Socket();
}

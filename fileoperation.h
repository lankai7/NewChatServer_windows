#pragma once
#ifndef __FILEOP_H__
#define  __FILEOP_H__
#include "tcpSocket.h"

class fileoperation{
public:
    long g_fileSize;	//保存文件大小
    string g_fileBuf;	//保存文件数据

    //读取文件（读）
    bool readFile(const char* fileName);

    //发文件
    bool sendFile(SOCKET s);

    /*******客户端********/
    //接收文件
    bool recvFile(SOCKET s);

    //保存文件（写）
    bool saveFile(const char* fileName);

    //获取数据
    void getSize(long num);
};


#endif

#pragma once
#ifndef __FILEOP_H__
#define  __FILEOP_H__
#include "tcpSocket.h"

/*******服务器********/
//读取文件（读）
bool readFile(const char* fileName);

//发文件
bool sendFile(SOCKET s, const char* fileName);

/*******客户端********/
//接收文件
bool recvFile(SOCKET s, const char* fileName);

//保存文件（写）
bool saveFile(const char* fileName);

#endif

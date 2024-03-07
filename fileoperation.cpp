#include "fileOperation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;
long g_fileSize;	//保存文件大小
string g_fileBuf;	//保存文件数据

bool readFile(const char* fileName)
{
    ifstream read;
    read.open(fileName, ios::binary);
    if (!read) {
        cout << "file open failed: read" << endl;
        return false;
    }

    //把文件读到内存中来
    ostringstream contents;
    contents << read.rdbuf();
    g_fileBuf = contents.str();
    //获取文件大小
    g_fileSize = g_fileBuf.length();

    read.close();
    return true;
}

bool sendFile(SOCKET s, const char* fileName)
{
    readFile(fileName);
    const char* buf = g_fileBuf.c_str();
    int ret = send(s, buf, g_fileSize, 0);
    if (ret == SOCKET_ERROR) {
        err("sendFile");
        return false;
    }
    cout << "发送成功：" << g_fileSize << "Byte" << endl;

    return false;
}

bool recvFile(SOCKET s, const char* fileName)
{
    char* buf;
    buf = new char[g_fileSize];
    int ret = recv(s, buf, g_fileSize, 0);
    if (ret == 0) {
        cout << "服务器正常" << endl;
    }
    else if (ret < 0) {
        err("recv");
    }
    g_fileBuf = buf;
    saveFile(fileName);

    delete[] buf;
    return false;
}

bool saveFile(const char* fileName)
{
    ofstream write;
    write.open(fileName, ios::binary);
    if (!write) {
        cout << "file open failed: write" << endl;
        return false;
    }

    write << g_fileBuf;

    write.close();
    return true;
}

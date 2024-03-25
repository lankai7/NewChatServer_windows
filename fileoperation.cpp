#include "fileOperation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

bool fileoperation::readFile(const char* fileName)
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

bool fileoperation::sendFile(SOCKET s)
{
    const char* buf = g_fileBuf.c_str();
    int ret = send(s, buf, g_fileSize, 0);
    if (ret == SOCKET_ERROR) {
        err("sendFile");
        return false;
    }
    cout << "发送成功：" << g_fileSize << "Byte" << endl;

    return false;
}

bool fileoperation::recvFile(SOCKET s)
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
    saveFile("fileName");
    delete[] buf;
    return false;
}

bool fileoperation::saveFile(const char* fileName)
{
    ofstream write;
    write.open("fileName.png", ios::binary);
    if (!write) {
        cout << "file open failed: write" << endl;
        return false;
    }

    write << fileName;

    write.close();
    return true;
}

void fileoperation::getSize(long num)
{
    g_fileSize = num;
}



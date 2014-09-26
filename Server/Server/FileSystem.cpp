//
//  FileSystem.cpp
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "FileSystem.h"

FileSystem::FileSystem() {
    strcpy(DIRECTORY_ROOT, "/Users/artur/http-test-suite/");
}

FileSystem::FileSystem(const std::string & path) {
    const char* c_path = path.c_str();
    strcpy(DIRECTORY_ROOT, c_path);
    
}

bool FileSystem::fileExist(const std::string & path) {
    const char* c_path = path.c_str();
    int status = initSt(c_path);
    if( status != 0 )
        return false;
    else
        return true;
}

bool FileSystem::isDirectory(const std::string & path) {
    const char* c_path = path.c_str();
    initSt(c_path);
    if (S_ISDIR (st.st_mode))
        return true;
    else
        return false;
}

bool FileSystem::isFile(const std::string & path) {
    const char* c_path = path.c_str();
    initSt(c_path);
    if (S_ISREG (st.st_mode))
        return true;
    else
        return false;
}

unsigned FileSystem::getLength(const std::string & path) {
    const char* c_path = path.c_str();
    initSt(c_path);
    return (int)st.st_size;  // Do i need -1 ?
}

std::string FileSystem::getContentType(const std::string & path) {
    size_t pos = path.find_last_of(".");
    std::string type = "";
    if(pos == std::string::npos)
        return type;
    for(unsigned i = (int)pos; i < path.length(); i++)
        type += path[i];
    return type;
}

char * FileSystem::getFile(const std::string & path) {
    char *absPath = new char[512];
    strcpy(absPath, DIRECTORY_ROOT);
    strcat(absPath, path.c_str() );
    std::ifstream stream(absPath, std::ios::in | std::ios::binary);
    size_t fileSize = this->getLength(path);
    char *buf = new char[fileSize];
    stream.read(buf, fileSize);
    stream.close();
    delete[] absPath;
    return buf;
}

int FileSystem::initSt(const char *path) {
    char *absPath = new char[512];
    strcpy(absPath, DIRECTORY_ROOT);
    strcat(absPath, path);
    int status = stat(absPath, &st);
    if(status != 0) {
        st.st_blksize = 0;
        st.st_blocks = 0;
        st.st_dev = 0;
        st.st_gid = 0;
        st.st_ino = 0;
        st.st_mode = 0;
        st.st_nlink = 0;
        st.st_rdev = 0;
        st.st_size = 0;
        st.st_uid = 0;
        //st.__pad0 = 0;
    }
    delete[] absPath;
    return status;
}
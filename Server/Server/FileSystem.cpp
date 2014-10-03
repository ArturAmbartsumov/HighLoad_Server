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
    const char* char_path = path.c_str();
    strcpy(DIRECTORY_ROOT, char_path);
}

bool FileSystem::pathExist(const std::string &path) {
    const char* char_path = path.c_str();
    int status = setStat(char_path);
    dropStat();
    return 0 == status;
}

bool FileSystem::isDirectory(const std::string & path) {
    const char* char_path = path.c_str();
    setStat(char_path);
    bool temp = S_ISDIR(st.st_mode);
    dropStat();
    return temp;
}

bool FileSystem::isFile(const std::string & path) {
    const char* char_path = path.c_str();
    setStat(char_path);
    bool temp = S_ISREG(st.st_mode);
    dropStat();
    return temp;
}

unsigned FileSystem::getFileLength(const std::string &path) {
    const char* char_path = path.c_str();
    setStat(char_path);
    unsigned int temp = (unsigned)st.st_size;
    dropStat();
    return temp;
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

File FileSystem::getFile(const std::string &path) {
    char *fileName = new char[FILENAME_MAX];
    strcpy(fileName, DIRECTORY_ROOT);
    strcat(fileName, path.c_str() );
    int fd = open(fileName, O_RDWR);
    File file(fd, getFileLength(path));
    return file;
}

int FileSystem::setStat(const char *path) {
    char *fileName = new char[FILENAME_MAX];
    strcpy(fileName, DIRECTORY_ROOT);
    strcat(fileName, path);
    int status = stat(fileName, &st);
    if(status != 0) {
        dropStat();
    }
    delete[] fileName;
    return status;
}

void FileSystem::dropStat() {
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
}
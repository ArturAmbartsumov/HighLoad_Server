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
    bool temp = S_ISDIR(_stat.st_mode);
    dropStat();
    return temp;
}

bool FileSystem::isFile(const std::string & path) {
    const char* char_path = path.c_str();
    setStat(char_path);
    bool temp = S_ISREG(_stat.st_mode);
    dropStat();
    return temp;
}

unsigned FileSystem::getFileLength(const std::string &path) {
    const char* char_path = path.c_str();
    setStat(char_path);
    unsigned int temp = (unsigned) _stat.st_size;
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
    int status = stat(fileName, &_stat);
    if(status != 0) {
        dropStat();
    }
    delete[] fileName;
    return status;
}

void FileSystem::dropStat() {
    _stat.st_blksize = 0;
    _stat.st_blocks = 0;
    _stat.st_dev = 0;
    _stat.st_gid = 0;
    _stat.st_ino = 0;
    _stat.st_mode = 0;
    _stat.st_nlink = 0;
    _stat.st_rdev = 0;
    _stat.st_size = 0;
    _stat.st_uid = 0;
}
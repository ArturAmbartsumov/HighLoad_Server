//
//  FileSystem.h
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__FileSystem__
#define __Server__FileSystem__

#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <iostream>
//#include <sys/fcntl.h>
#include "File.h"

class FileSystem {
    
public:
    FileSystem();
    FileSystem(const std::string & path);
    
    bool pathExist(const std::string &path);
    bool isDirectory(const std::string & path);
    bool isFile(const std::string & path);
    unsigned getFileLength(const std::string &path);
    std::string getContentType(const std::string & path);
    File getFile(const std::string &path);
    void dropStat();
    
private:
    char DIRECTORY_ROOT[FILENAME_MAX];
    struct stat _stat;
    
    int setStat(const char *path);
};

#endif /* defined(__Server__FileSystem__) */

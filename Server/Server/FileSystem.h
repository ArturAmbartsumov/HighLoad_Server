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
#include <string>
#include <cstring>
#include <iostream>

class FileSystem {
    
public:
    FileSystem();
    FileSystem(const std::string & path);
    
    bool fileExist(const std::string & path);
    bool isDirectory(const std::string & path);
    bool isFile(const std::string & path);
    unsigned getLength(const std::string & path);
    std::string getContentType(const std::string & path);
    char * getFile(const std::string & path);
    
private:
    char DIRECTORY_ROOT[512];
    struct stat st;
    
    int initSt(const char *path);
};

#endif /* defined(__Server__FileSystem__) */

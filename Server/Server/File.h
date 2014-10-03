//
//  File.h
//  Server
//
//  Created by Артур on 03.10.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__File__
#define __Server__File__

#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>

class File {
public:
    const unsigned int fileDesc;
    const unsigned int fileLength;

    File(int fd, int fl);
    void closeFile();
};

#endif /* defined(__Server__File__) */

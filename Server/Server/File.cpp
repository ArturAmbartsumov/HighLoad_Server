//
//  File.cpp
//  Server
//
//  Created by Артур on 03.10.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "File.h"

File::File(int fd, int fl): fileDesc(fd), fileLength(fl) {
}

void File::closeFile() {
    close(fileDesc);
}
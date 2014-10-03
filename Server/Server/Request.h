//
//  Request.h
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__Request__
#define __Server__Request__

#include <iostream>
#include <string>
#include "UrlParser.h"

class Request {
    
public:
    Request(char* requestBuffer);
    std::string getMethod();
    std::string getUrl();
    void addIndex();
    std::string getHttpVersion();
    void print();
    
private:
    std::string method;
    std::string url;
    std::string httpVersion;
};

#endif /* defined(__Server__Request__) */

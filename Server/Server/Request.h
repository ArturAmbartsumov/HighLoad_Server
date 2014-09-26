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

class Request {
    
public:
    Request(char* requestBuffer);
    std::string getRequestMethod();
    std::string getRequestUrl();
    std::string getHttpVersion();
    void print();
    
private:
    std::string requestMethod;
    std::string requestUrl;
    std::string httpVersion;
};

#endif /* defined(__Server__Request__) */

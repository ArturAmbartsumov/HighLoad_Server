//
//  Response.h
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__Response__
#define __Server__Response__

#include <string>
#include <time.h>
#include <sstream>
#include <iostream>

class Response {
    
public:
    Response();
    void setDate();
    void setStatusCode(int status);
    void setHttpVersion(std::string version);
    void setConnection(std::string connectionStatus);
    void setContentType(std::string type);
    void setContentLength(unsigned size);
    std::string getResponse();
    std::string getStatusCode();
    
private:
    std::string statusCode;
    std::string httpVersion;
    std::string connection;
    std::string date;
    std::string contentType;
    unsigned contentLength;
};

#endif /* defined(__Server__Response__) */

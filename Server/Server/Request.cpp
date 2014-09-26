//
//  Request.cpp
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "Request.h"

Request::Request(char* requestBuffer) {
    requestMethod = "";
    requestUrl = "";
    httpVersion = "";
    
    while(*requestBuffer != ' ')
        requestMethod += *requestBuffer++;
    
    requestBuffer++;
    while(*requestBuffer != ' ')
        requestUrl += *requestBuffer++;
    
    requestBuffer++;
    while(*requestBuffer != '\n')
        httpVersion += *requestBuffer++;
}

std::string Request::getRequestMethod() {
    return requestMethod;
}
std::string Request::getRequestUrl() {
    return requestUrl;
}
std::string Request::getHttpVersion() {
    return httpVersion;
}

void Request::print() {
    //std::cout << std::endl << "requestMethod: "<< requestMethod
   // << std::endl << "requestUrl: "<< requestUrl
    //<< std::endl << "httpVersion: " << httpVersion
    //<< std::endl;
}

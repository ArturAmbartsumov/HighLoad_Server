//
//  Request.cpp
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "Request.h"

Request::Request(char* requestBuffer) {
    method = "";
    url = "";
    httpVersion = "";
    
    while(*requestBuffer != ' ')
        method += *requestBuffer++;
    
    requestBuffer++;
    while(*requestBuffer != ' ')
        url += *requestBuffer++;
    url = urlDeleteParams( urlDecoder(url) );
    
    requestBuffer++;
    while(*requestBuffer != '\n')
        httpVersion += *requestBuffer++;

}

std::string Request::getMethod() {
    return method;
}

std::string Request::getUrl() {
    return url;
}

void Request::addIndex() {
    url += "/index.html";
}

std::string Request::getHttpVersion() {
    return httpVersion;
}

void Request::print() {
    std::cout << std::endl << "method: "<< method
    << std::endl << "url: "<< url
    << std::endl << "httpVersion: " << httpVersion
    << std::endl;
}

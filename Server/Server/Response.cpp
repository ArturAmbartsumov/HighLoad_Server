//
//  Response.cpp
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "Response.h"

Response::Response() {
    connection = "close";
}

void Response::setDate() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,80,"%a, %d %b %G %T GMT",timeinfo);
    
    std::string dateStr(buffer);
    date = dateStr;
}

void Response::setStatusCode(int status) {
    switch(status) {
        case 200:
            statusCode = "200 OK";
            break;
        case 404:
            statusCode = "404 Not Found";
            break;
        case 405:
            statusCode = "405 METHOD NOT ALLOWED";
            break;
        case 403:
            statusCode = "403 FORBIDDEN";
            break;
        default:
            statusCode = "405 METHOD NOT ALLOWED";
    }
}

void Response::setConnection(std::string connectionStatus) {
    connection = connectionStatus;
}

void Response::setContentLength(unsigned size) {
    contentLength = size;
}

void Response::setHttpVersion(std::string version) {
    httpVersion = version;
}

void Response::setContentType(std::string type) {
    if(type == ".html")
        contentType = "text/html";
    if(type == ".css")
        contentType = "text/css";
    if(type == ".js")
        contentType = "text/javascript";
    if(type == ".jpg" || type == ".jpeg")
        contentType = "image/jpeg";
    if(type == ".gif")
        contentType = "image/gif";
    if(type == ".png")
        contentType = "image/png";
    if(type == ".txt")
        contentType = "text/plain";
    if(type == ".swf")
        contentType = "application/x-shockwave-flash";
}

std::string Response::getResponse(){
    std::stringstream ss;
    ss << "HTTP/1.1 " << statusCode << "\r\n"
    << "Date: " << date << "\r\n"
    << "Server: MEGASERVER\r\n" ;
    if(statusCode == "200 OK") {
        ss << "Content-Type: " << contentType << "\r\n";
    }
    ss  << "Content-Length: " << contentLength << "\r\n"
    << "Connection: " << connection << "\r\n\r\n";
    std::string responseStr = ss.str();
    return responseStr;
}

std::string Response::getStatusCode() {
    return statusCode;
}

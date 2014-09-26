//
//  Worker.cpp
//  Server
//
//  Created by Артур on 22.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "Worker.h"

Worker::Worker() {
    clientsNumber = 0;
    notified = false;
    //testNumber = rand();
}

unsigned long Worker::getClientsNumber() {
    return clientsNumber;
}

void Worker::pushClient(int acceptedFileDescriptor) {
    clients.push(acceptedFileDescriptor);
    clientsNumber = clients.getClientsNumber();
    wakeUp();
}

int Worker::popClient() {
    int buff = clients.pop();
    clientsNumber = clients.getClientsNumber();
    return buff;
}

void Worker::execute(int acceptedFileDescriptor) {
    
    char buffer[1024];
    
    bool readingData = true;
    while( readingData ) {
        int bytesReaded = (int)recv(acceptedFileDescriptor, &buffer, 1024, 0);
        if(bytesReaded == 0) {
            std::cout << "Reading data error" << std::endl;
            break;
        }
        readingData = memcmp(buffer + bytesReaded - 4, "\r\n\r\n", 4);
    }
    
    Request r(buffer);
    r.print();
    //std::cout << "\n\n";
    
    Response resp;
    char* buf = NULL;
    FileSystem *fs = new FileSystem();
    
    std::string url =  urlDeleteParams( urlDecoder(r.getRequestUrl()) );
    std::string method = r.getRequestMethod();
    
    if( !(method == "GET" || method == "HEAD") ) {
        resp.setStatusCode(405);
        resp.setContentLength(0);
        resp.setConnection("close");
        resp.setDate();
    }
    else {
        if( !fs->fileExist(url)) {
            //std::cout << "HEREEE";
            resp.setStatusCode(404);
            resp.setContentLength(0);
            resp.setConnection("close");
            resp.setDate();
        }
        if(fs->isDirectory(url)) {
            url += "/index.html";
            if(!fs->fileExist(url)) {
                resp.setStatusCode(403);
                resp.setContentLength(0);
                resp.setConnection("close");
                resp.setDate();
            }
        }
        
        if(fs->isFile( url )) {
            size_t fileSize = fs->getLength( url ) ;
            resp.setDate();
            resp.setStatusCode(200);
            resp.setContentLength((int)fileSize);
            resp.setContentType( fs->getContentType( url ) );
            resp.setConnection("close");
            
        }
        
    }
    std::string respStr = resp.getResponse();
    //std::cout << respStr;
    send(acceptedFileDescriptor, respStr.c_str(), respStr.length(), 0);
    if(method == "GET" && resp.getStatusCode() == "200 OK" ) {
        size_t fileSize = fs->getLength( url ) ;
        char *buf = fs->getFile( url );
        if(buf != NULL)
            send(acceptedFileDescriptor, buf, fileSize, 0);
    }
    //std::cout << "\n\n";
    
    delete[] buf;
    delete fs;
    buf = NULL;
    
    //std::this_thread::sleep_for(std::chrono::seconds(5));
    /*{
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        std::cout << std::endl << " Клиент : " << acceptedFileDescriptor << " обслужился" << std::endl;
        std::cout << "Размер очереди " << clientsNumber << std::endl;
    }*/
    close(acceptedFileDescriptor);
}

void Worker::run(int workerIndex) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        std::cout << "Поток " << workerIndex << " запущен " << std::endl;
    }
    this->workerIndex = workerIndex;
    while (1) {
        std::unique_lock<std::mutex> locker(_lock);
        while(!notified) { // от ложных пробуждений
            g_queuecheck.wait(locker);
        }
        while (getClientsNumber()) {
            execute(popClient());
        }
        notified = false;
            //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Worker::wakeUp() {
    {
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        //std::cout << "Поток " << workerIndex << " просыпается" << std::endl;
        //std::cout << "Размер очереди " << clientsNumber << std::endl << std::endl;
    }
    notified = true;
    g_queuecheck.notify_all();
}
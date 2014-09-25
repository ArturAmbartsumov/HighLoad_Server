//
//  Server.cpp
//  Server
//
//  Created by Артур on 22.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "Server.h"

Server::Server(int port, int workersNumber): workers(workersNumber) {
    this->workersNumber = workersNumber;
    createSocket();
    sockaddr_in socketAddres = createSocketAdress(port);
    bindSocket(socketAddres);
}

Server::~Server() {
    
}

void Server::start() {
    {
        std::unique_lock<std::mutex> locker(g_lockprint);
        std::cout << "Listen socket: ";
        int listenStatus = listen(socketFileDescriptor, 5);
        if(listenStatus == -1) {
            std::cout << "Listen Error" << std::endl;
            //возбудить эксепшн
        }
    
        std::cout << "Done" << std::endl<<std::endl;
    }
    
    initWorkers();
    
    while(1) {
        int acceptedFileDescriptor = accept(socketFileDescriptor, NULL, NULL);
        {
            std::unique_lock<std::mutex> locker(g_lockprint);
            if(acceptedFileDescriptor == -1) {
                std::cout << "Accept connected client error" << std::endl;
                //break;
            }
            //std::cout << std::endl << "Client connected : " << acceptedFileDescriptor << std::endl << std::endl;
        }
        
        int freeWorker = getFreeWorker();
        workers[freeWorker].pushClient(acceptedFileDescriptor);
        
        /*char buffer[1024];
        
        bool readingData = true;
        while( readingData ) {
            int bytesReaded = (int)recv(acceptedFileDescriptor, &buffer, 1024, 0);
            if(bytesReaded == 0) {
                std::cout << "Reading data error" << std::endl;
                break;
            }
            readingData = memcmp(buffer + bytesReaded - 4, "\r\n\r\n", 4);
        }
        for(int i = 0; buffer[i] != '\0'; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl << std::endl;
        
        char msg[] = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\nConnection: close\r\n\r\nHello world!r\n\r\n";
        send(acceptedFileDescriptor, msg, 1024 , 0);*/
    }
}

void Server::createSocket() {
    std::unique_lock<std::mutex> locker(g_lockprint);
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "Init socket : ";
    if(socketFileDescriptor < 0) {
        std::cout << "Socket Initialization Failed" << std::endl;
        //возбудить эксепшн
    }
    std::cout << "Done" << std::endl;
}

sockaddr_in Server::createSocketAdress(int port) {
    sockaddr_in socketAddres;
    socketAddres.sin_family = AF_INET;
    socketAddres.sin_port = htons(port);
    socketAddres.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return socketAddres;
}

void Server::bindSocket(sockaddr_in socketAddres) {
    std::unique_lock<std::mutex> locker(g_lockprint);
    std::cout << "Bind socket : ";
    int bindStatus = bind(socketFileDescriptor, (sockaddr *) &socketAddres, sizeof(sockaddr));
    if(bindStatus == -1) {
        std::cout << "Bind Error" << std::endl;
        //возбудить эксепшн
    }
    std::cout << "Done" << std::endl;
}

void Server::pushClient(int acceptedFileDescriptor) {
    
}

void Server::initWorkers() {
    for (int i = 0; i < workers.size(); i++) {
        std::thread thr(&Worker::run, &workers[i], i);
        thr.detach();
    }
}

int Server::getFreeWorker() {
    int buffWorker = 0;
    for (int i = 1; i < workers.size(); i++) {
        if (workers[buffWorker].getClientsNumber() > workers[i].getClientsNumber()) {
            buffWorker = i;
        }
    }
    return buffWorker;
}
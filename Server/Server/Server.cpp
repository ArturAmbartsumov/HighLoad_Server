//
//  Server.cpp
//  Server
//
//  Created by Артур on 22.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "Server.h"

Server::Server(int port, int workersNumber, int maxQueue): workers(workersNumber) {
    this->workersNumber = workersNumber;
    this->maxQueue = maxQueue;
    createSocket();
    sockaddr_in socketAddres = createSocketAdress(port);
    bindSocket(socketAddres);
}

Server::~Server() {
    close(this->socketFileDescriptor);
}

void Server::start() {
    signal(SIGPIPE, SIG_IGN);
    
    startListen();
    
    initWorkers();

    while(1) {
        int clientSocket = accept(socketFileDescriptor, NULL, NULL);
        
        if(clientSocket == -1) {
            std::unique_lock<std::mutex> locker(g_lockprint);
            std::cout << "Accept connected client error" << std::endl;
            break;
        }

        int freeWorker = getFreeWorker();
        workers[freeWorker].pushClient(clientSocket);

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
    else
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
    else
        std::cout << "Done" << std::endl;
}

void Server::startListen() {
    std::unique_lock<std::mutex> locker(g_lockprint);
    std::cout << "Listen socket: ";
    int listenStatus = listen(socketFileDescriptor, maxQueue);
    if(listenStatus == -1) {
        std::cout << "Listen Error" << std::endl;
        //возбудить эксепшн
    }
    else
        std::cout << "Done" << std::endl;
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
//
//  Server.h
//  Server
//
//  Created by Артур on 22.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__Server__
#define __Server__Server__

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <mutex>

#include "Worker.h"

class Server {
    
private:
    int port;
    int maxQueue;
    int socketServer;
    std::vector<Worker> workers;
    int workersNumber;
    std::mutex g_lockprint;
    
    void createSocket();
    sockaddr_in createSocketAdress(int port);
    void bindSocket(sockaddr_in socketAddres);
    void startListen();
    void initWorkers();
    void killWorkers();
    int getFreeWorker();
    
public:
    Server(int port, int workersNumber, int maxQueue);
    ~Server();
    void start();
};



#endif /* defined(__Server__Server__) */

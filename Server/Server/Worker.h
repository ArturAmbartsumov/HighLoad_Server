//
//  Worker.h
//  Server
//
//  Created by Артур on 22.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__Worker__
#define __Server__Worker__


#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "ClientsQueue.h"
#include "Request.h"
#include "Response.h"
#include "FileSystem.h"
#include "UrlParser.h"
#include "File.h"

static std::recursive_mutex g_lockprint;

class Worker {
    
private:
    std::mutex  _lock;
    ClientsQueue clients;
    unsigned long clientsNumber;
    int workerIndex;
    bool stopThread;
    FileSystem fs;

    void execute(int clientSocket);
    int popClient();
    Response buildResponse(Request &r);
    
public:
    int testNumber;
    bool notified;
    std::condition_variable g_queuecheck;
    
    Worker();
    ~Worker();
    void shutDown();
    void run(int t);
    void wakeUp();
    unsigned long getClientsNumber();
    void pushClient(int clientSocket);
};

#endif /* defined(__Server__Worker__) */

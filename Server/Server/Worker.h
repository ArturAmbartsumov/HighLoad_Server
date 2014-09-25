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

static std::recursive_mutex g_lockprint;

class Worker {
    
private:
    std::mutex  _lock;
    ClientsQueue clients;
    unsigned long clientsNumber;
    int workerIndex;

    void execute(int acceptedFileDescriptor);
    int popClient();
    
public:
    int testNumber;
    bool notified;
    std::condition_variable g_queuecheck;
    
    Worker();
    void run(int t);
    void wakeUp();
    unsigned long getClientsNumber();
    void pushClient(int acceptedFileDescriptor);
};

#endif /* defined(__Server__Worker__) */

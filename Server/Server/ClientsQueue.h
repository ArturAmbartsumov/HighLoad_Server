//
//  ClientsQueue.h
//  Server
//
//  Created by Артур on 24.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__ClientsQueue__
#define __Server__ClientsQueue__


#include <mutex>
#include <queue>

class ClientsQueue {
    
private:
    std::recursive_mutex _lock;
    std::queue<int> clients;
    unsigned long clientsNumber;
    
public:
    ClientsQueue();
    void push(int client);
    int pop();
    unsigned long getClientsNumber();
};

#endif /* defined(__Server__ClientsQueue__) */

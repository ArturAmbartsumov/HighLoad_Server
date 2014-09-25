//
//  ClientsQueue.cpp
//  Server
//
//  Created by Артур on 24.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "ClientsQueue.h"

ClientsQueue::ClientsQueue() {
    clientsNumber = 0;
}

void ClientsQueue::push(int client) {
    std::lock_guard<std::recursive_mutex> locker(_lock);
    clients.push(client);
    clientsNumber = clients.size();
}

int ClientsQueue::pop() {
    std::lock_guard<std::recursive_mutex> locker(_lock);
    int client = clients.front();
    clients.pop();
    clientsNumber = clients.size();
    return client;
}

unsigned long ClientsQueue::getClientsNumber() {
    std::lock_guard<std::recursive_mutex> locker(_lock);
    return clientsNumber;
}
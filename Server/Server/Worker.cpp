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
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    {
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        std::cout << std::endl << " Клиент : " << acceptedFileDescriptor << " обслужился" << std::endl;
        std::cout << "Размер очереди " << clientsNumber << std::endl;
    }
    close(acceptedFileDescriptor);
    /*{
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        std::cout << std::endl << number << " Client connected : " << acceptedFileDescriptor << std::endl << std::endl;
    }
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
    for(int i = 0; buffer[i] != '\0'; i++) {
        std::cout << buffer[i];
    }
    std::cout << std::endl << std::endl;
    
    char msg[] = "HTTP/1.1 200 OK\r\nContent-Length: 14\r\nConnection: close\r\n\r\nHello world!r\n\r\n";
    send(acceptedFileDescriptor, msg, 1024 , 0);
    close(acceptedFileDescriptor);*/
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
        std::cout << "Поток " << workerIndex << " просыпается" << std::endl;
        std::cout << "Размер очереди " << clientsNumber << std::endl << std::endl;
    }
    notified = true;
    g_queuecheck.notify_all();
}
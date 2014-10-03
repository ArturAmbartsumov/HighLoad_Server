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
    stopThread = false;
}

Worker::~Worker() {
    //shutDown();
    //std::this_thread::sleep_for(std::chrono::seconds(5));
}

void Worker::shutDown() {
}

unsigned long Worker::getClientsNumber() {
    //return clientsNumber;
    return clients.getClientsNumber();
}

void Worker::pushClient(int clientSocket) {
    clients.push(clientSocket);
    clientsNumber = clients.getClientsNumber();
    //if (clientsNumber < 2)
        wakeUp();
}

int Worker::popClient() {
    int buff = clients.pop();
    clientsNumber = clients.getClientsNumber();
    return buff;
}

void Worker::execute(int clientSocket) {

    char buffer[1024];


        int bytesReaded = (int)recv(clientSocket, &buffer, 1024, 0);
        if(bytesReaded == 0) {
            std::unique_lock<std::recursive_mutex> locker(g_lockprint);
            std::cout << "Reading data error" << workerIndex << std::endl;
        }
    
    
    Request r(buffer);
    Response resp = buildResponse(r);
    
    //Отправка заголовков
    std::string respStr = resp.getResponse();
    ssize_t s = send(clientSocket, respStr.c_str(), respStr.length(), 0);
    if (s == -1 || errno == EPIPE) {
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        std::cout << "EPIPE при отправке заголовков, errno =  " << errno << "\n";
        close(clientSocket);
        return;
    }
    
    //Отправка данных, если они есть
    if(r.getMethod() == "GET" && resp.getStatusCode() == "200 OK" ) {
        File file = fs.getFile(r.getUrl());
        off_t offset = 0;
        off_t fileLength = file.fileLength;
        int ret = sendfile(file.fileDesc, clientSocket, offset, &fileLength, NULL, 0);
        if (ret == -1) {
            if (errno == EAGAIN) {
                if (fileLength == 0) {
                    std::cout << "Didn't send anything. Return error with errno == EAGAIN" << "\n";
                } else {
                    std::cout << "We sent some bytes, but they we would block" << "\n";
                }
            } else if (errno == EPIPE) {
                std::unique_lock<std::recursive_mutex> locker(g_lockprint);
                std::cout << "EPIPE при отправке данных, errno =  " << errno << "\n";
                file.closeFile();
                close(clientSocket);
                return;
            } else std::cout << "Some other error " << errno << "\n";
        }
        file.closeFile();
    }
    close(clientSocket);
}

Response Worker::buildResponse(Request &r) {
    Response resp;
    //std::string url =  urlDeleteParams( urlDecoder(r.getUrl()) );
    std::string method = r.getMethod();
    if(method == "GET" || method == "HEAD") {
        if(!fs.pathExist(r.getUrl())) {
            resp.setStatusCode(404);
            resp.setContentLength(0);
        }
        else {
            if (fs.isDirectory(r.getUrl())) {
                r.addIndex();
                if (!fs.pathExist(r.getUrl())) {
                    resp.setStatusCode(403);
                    resp.setContentLength(0);
                }
            }
            if (fs.isFile(r.getUrl())) {
                resp.setStatusCode(200);
                unsigned int fl = fs.getFileLength(r.getUrl());
                resp.setContentLength(fl);
                resp.setContentType(fs.getContentType(r.getUrl()));
            }
        }
    }
    else {
        resp.setStatusCode(405);
        resp.setContentLength(0);
    }
    resp.setConnection("close");
    resp.setDate();
    return resp;
}

void Worker::run(int workerIndex) {
    signal(SIGPIPE, SIG_IGN);
    {
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        std::cout << "Поток " << workerIndex << " запущен " << std::endl;
    }
    this->workerIndex = workerIndex;
    
    while (1) {
        {
            std::unique_lock<std::mutex> locker(_lock);
            while(!notified) { // от ложных пробуждений
                g_queuecheck.wait(locker);
            }
            notified = false;
        }
        
        while (getClientsNumber()) {
            execute(popClient());
        }
        
        
    }
}

void Worker::wakeUp() {
    /*{
        std::unique_lock<std::recursive_mutex> locker(g_lockprint);
        std::cout << "Поток " << workerIndex << " просыпается" << std::endl;
        std::cout << "Размер очереди " << clientsNumber << std::endl << std::endl;
    }*/
    std::unique_lock<std::mutex> locker(_lock);
    notified = true;
    g_queuecheck.notify_one();
}
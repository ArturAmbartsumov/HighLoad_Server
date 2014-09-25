//
//  main.cpp
//  Server
//
//  Created by Артур on 22.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include <iostream>
#include "Server.h"
#include <thread>
#include <mutex>


int main() {
    Server server(8080, 10);
    //std::cout << "Hello world";
    std::thread main_thread(&Server::start, &server);
    //std::cout << "Hello world";
    main_thread.join();
    //std::cout << "Hello world";
    return 0;
}


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
#include <string>
#include <signal.h>


int main() {
    signal(SIGPIPE, SIG_IGN);
    Server server(8000, 3, 1000);
    std::thread main_thread(&Server::start, &server);
    main_thread.join();
    return 0;
}


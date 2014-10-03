cmake_minimum_required(VERSION 2.8.4)
project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    ClientsQueue.cpp
    ClientsQueue.h
    Decoder.cpp
    Decoder.h
    FileSystem.cpp
    FileSystem.h
    main.cpp
    Request.cpp
    Request.h
    Response.cpp
    Response.h
    Server.cpp
    Server.h
    Worker.cpp
    Worker.h)

add_executable(Server ${SOURCE_FILES})
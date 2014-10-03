//
//  Decoder.h
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#ifndef __Server__UrlParser__
#define __Server__UrlParser__

#include <iostream>
#include <string>
#include <cmath>
#include <cstring>

bool checkEntry(int num, int left, int right) ;

int hexToDec(const std::string & code) ;

std::string urlDecoder(const std::string & url);

std::string urlDeleteParams(const std::string & url);

#endif /* defined(__Server__Decoder__) */

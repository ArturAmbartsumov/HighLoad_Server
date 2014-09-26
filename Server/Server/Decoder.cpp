//
//  Decoder.cpp
//  Server
//
//  Created by Артур on 26.09.14.
//  Copyright (c) 2014 Артур. All rights reserved.
//

#include "Decoder.h"

bool checkEntry(int num, int left, int right) {
    if ( num >= left && num <= right)
        return true;
    else
        return false;
}

int hexToDec(const std::string & code) {
    int digit = 0, result = 0;
    const int base = 16;
    for(unsigned int i = 0; i < code.length(); i++) {
        if ( checkEntry(code[i], '0' , '9') )
            digit = code[i] - '0';
        if( checkEntry(code[i], 'a', 'f') )
            digit = code[i] - 'a' + 10; // a = 10, f = 15
        if( checkEntry(code[i], 'A', 'F') )
            digit = code[i] - 'A' + 10; //A = 10, F = 15
        result += digit * pow(base, code.length() - i - 1);
    }
    return result;
}

std::string urlDecoder(const std::string & url) {
    std::string resultUrl = "";
    int urlPos = 0;
    std::string hexCode = "";
    
    while(url[urlPos] != '\0') {
        if(url[urlPos] != '%') {
            resultUrl += url[urlPos];
        }
        else {
            hexCode += url[++urlPos]; //next two chars in code %XX
            hexCode += url[++urlPos];
            resultUrl += (char) hexToDec(hexCode);
        }
        urlPos++;
    }
    return resultUrl;
}

std::string urlDeleteParams(const std::string & url) {
    std::string newUrl;
    size_t dots = url.find("..");
    if( dots != std::string::npos ){
        newUrl = "kokokokokokokokokokokokokokodaaaaaaaa";
        return newUrl;
    }
    size_t sign = url.find("?");
    if( sign == std::string::npos )
        return url;
    newUrl = url.substr(0,sign);
    return newUrl;
}

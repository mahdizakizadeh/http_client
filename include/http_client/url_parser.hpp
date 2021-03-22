//
//  url_parser.hpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/13/21.
//

#ifndef URL_PARSER_HPP_
#define URL_PARSER_HPP_

#include <stdio.h>
#include <string>

using std::string;

class UrlParser {

public:
    const char* protocol;
    const char* hostname;
    const char* port;
    const char* identifier;
    
    UrlParser(string url);
};

#endif /* url_parser_hpp */

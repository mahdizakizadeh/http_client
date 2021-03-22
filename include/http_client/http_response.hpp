//
//  http_response.hpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/12/21.
//

#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include <stdio.h>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class HttpResponse {
    
public:
    const int status;
    const unordered_map<string, string> headers;
    const string body;
    
    HttpResponse(int status,
                 const unordered_map<string, string>& headers,
                 const string& body);
};

#endif /* http_response_hpp */

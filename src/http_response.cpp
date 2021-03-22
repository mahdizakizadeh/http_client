//
//  http_response.cpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/12/21.
//

#include "http_response.hpp"

HttpResponse::HttpResponse(int status,
                           const unordered_map<string, string>& headers,
                           const string& body) : status(status), headers(headers), body(body) {};

//
//  http_request.hpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/12/21.
//

#ifndef HTTP_REQUEST_HPP_
#define HTTP_REQUEST_HPP_
#define MAX_SIZE 0x1000

#include <array>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include "http_request_method.hpp"
#include "http_response.hpp"
#include "url_parser.hpp"

using std::string;
using std::array;
using std::unordered_map;

class HttpRequest {
    int sock_fd;
    struct sockaddr_in sock_addr;
    
    int connect();
    
    ssize_t send(const string& buf);
    
    ssize_t recv(array<char, MAX_SIZE>& buf);
    
    const string generate_http_request();
    
    const string get_hostname_ip();
    
    const unordered_map<string, string> headers_str_to_map(string& header);
    
public:
    const UrlParser url_parser;
    const HttpRequestMethod httpRequestMethod;
    const unordered_map<string, string> headers;
    const unordered_map<string, string> params;
    const unordered_map<string, string> body;
    
    HttpRequest(const string& url,
                const HttpRequestMethod& method,
                const unordered_map<string, string>& headers,
                const unordered_map<string, string>& params,
                const unordered_map<string, string>& body);
    
    HttpRequest(const string& url, const HttpRequestMethod& method);
    
    HttpRequest(const string& url);
    
    ~HttpRequest(void);
    
    HttpResponse request();
};

#endif /* http_request_hpp */

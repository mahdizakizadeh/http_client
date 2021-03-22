//
//  http_request.cpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/12/21.
//

#include <regex>
#include <stdlib.h>
#include <typeinfo>
#include "http_exception.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "url_parser.hpp"

int HttpRequest::connect() {
    return ::connect(sock_fd,
                     reinterpret_cast<struct sockaddr*>(&sock_addr),
                     sizeof(sock_addr));
}

ssize_t HttpRequest::send(const string& buf) {
    return ::send(sock_fd, buf.data(), buf.length(), 0);
}

ssize_t HttpRequest::recv(array<char, MAX_SIZE>& buf) {
    return ::recv(sock_fd, buf.data(), buf.size() - 1, 0);
}

const string HttpRequest::generate_http_request() {
    string HttpRequestMethodMap[] {"GET", "POST", "PUT", "OPTIONS", "HEAD", "DELETE"};
    string request = "";
    request += HttpRequestMethodMap[httpRequestMethod];
    request += " ";
    request += url_parser.identifier;
    if (httpRequestMethod == HttpRequestMethod::kGet && params.size() > 0) {
        request += "?";
        for (auto& param : params) {
            request += param.first;
            request += "=";
            request += param.second;
            request += "&";
        }
        request.pop_back();
    }
    request += " HTTP/1.1\r\n";
    request += "Host: ";
    request += url_parser.hostname;
    request += "\r\n";
    for (auto& header : headers) {
        request += header.first;
        request += ": ";
        request += header.second;
        request += "\r\n";
    }
    if (httpRequestMethod == HttpRequestMethod::kPost) {
        // Creating json body from unordered_map
        string json = "{";
        for (auto& it : body) {
            json += it.first;
            json += ":";
            json += it.second;
            json += ",";
        }
        json.pop_back();
        json += "}";
        // Inserting data into http request
        request += "Content-Type: application/x-www-form-urlencoded";
        request += "Content-Length: ";
        request += json.length();
        request += "\r\n\r\n";
        request += json;
    }
    request += "\r\n\r\n";
    return request;
}

const string HttpRequest::get_hostname_ip() {
    struct hostent* he;
    struct in_addr** addr_list;
    
    if (he = ::gethostbyname(url_parser.hostname); he == NULL) {
        throw std::runtime_error("get_hostname_ip");
    }
    
    addr_list = reinterpret_cast<struct in_addr**>(he->h_addr_list);
    
    string ip {};
    for (int i = 0; addr_list[i] != NULL; i++) {
        ip = string(inet_ntoa(*addr_list[i]));
    }
    
    return ip;
}

const unordered_map<string, string> HttpRequest::headers_str_to_map(string& header) {
    unordered_map<string, string> headers_map;
    
    size_t eol_pos = 0;
    size_t entry_colon_pos = 0;
    string line;
    
    // Split lines by "\r\n" and then each entry by ": " and making map
    while ((eol_pos = header.find("\r\n")) != std::string::npos) {
        line = header.substr(0, eol_pos);
        
        entry_colon_pos = line.find(": ");

        string key = line.substr(0, entry_colon_pos);
        string val = line.substr(entry_colon_pos + 2);

        headers_map[key] = val;
        
        header.erase(0, eol_pos + 2);
    }
    // Last line
    entry_colon_pos = header.find(": ");
    string key = header.substr(0, entry_colon_pos);
    string val = header.substr(entry_colon_pos + 2);
    headers_map[key] = val;
    
    return headers_map;
}

HttpRequest::HttpRequest(const string& url,
                         const HttpRequestMethod& method,
                         const unordered_map<string, string>& headers,
                         const unordered_map<string, string>& params,
                         const unordered_map<string, string>& body) : url_parser(UrlParser(url)), httpRequestMethod(method), headers(headers), params(params), body(body) {
    /* Initating socket for IP-V4 (AF_INET)
     * and for a TCP (SOCK_STREAM) coonection
     * throws exception if fails */
    if (sock_fd = ::socket(AF_INET, SOCK_STREAM, 0); sock_fd == -1) {
        throw std::runtime_error(strerror(errno));
    }
    
    // Use this for local connections
    // sock_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sock_addr.sin_addr.s_addr = inet_addr(get_hostname_ip().c_str());
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(atoi(url_parser.port));

    if (connect() == -1) {
        throw std::runtime_error(strerror(errno));
    }
}

HttpRequest::HttpRequest(const string& url,
                         const HttpRequestMethod& method) : HttpRequest(url,
                                                                        method,
                                                                        unordered_map<string, string>(),
                                                                        unordered_map<string, string>(),
                                                                        unordered_map<string, string>()) {}

HttpRequest::HttpRequest(const string& url) : HttpRequest(url,
                                                          HttpRequestMethod::kGet,
                                                          unordered_map<string, string>(),
                                                          unordered_map<string, string>(),
                                                          unordered_map<string, string>()) {};

HttpResponse HttpRequest::request() {
    string sendbuf {generate_http_request()};
    array<char, MAX_SIZE> recvbuf {};
    string response_str ("");
        
    // Send Request
    if (send(sendbuf) == -1) {
        throw std::runtime_error(strerror(errno));
    }
    
    // Recieve Response
    ssize_t r {};
    while ((r = recv(recvbuf)) > 0) {
        response_str += recvbuf.data();
    }
    if (r == -1) {
        throw std::runtime_error(strerror(errno));
    }
    
    int status {0};
    size_t position_of_header_break {response_str.find("\r\n\r\n")};
    
    const string response_header {response_str.substr(0, position_of_header_break)};
    const string response_body {response_str.substr(position_of_header_break + 4)};
    
    std::regex match_status ("HTTP/\\d.\\d (\\d{3}) .+");
    std::smatch match;
    if (std::regex_search(response_header.begin(), response_header.end(), match, match_status)) {
        status = atoi(match[1].str().c_str());
    }
    
    if (status == 0) {
        throw std::runtime_error("invalid http response");
    }
    
    // Removing first line
    size_t position_of_second_line = response_header.find("\r\n") + 2;
    string response_header_tuples = response_header.substr(position_of_second_line);
        
    // Split lines by "\r\n" and then each entry by ": " and making map
    const unordered_map<string, string> response_headers_map = headers_str_to_map(response_header_tuples);
    
    HttpResponse response = HttpResponse(status, response_headers_map, response_body);
    
    switch (status) {
        case 400:
            throw HttpException("Bad Request", response);
        case 401:
            throw HttpException("Unauthorized", response);
        case 403:
            throw HttpException("Forbidden", response);
        case 404:
            throw HttpException("Not Found", response);
        case 418:
            throw HttpException("I'm a teapot", response);
        case 429:
            throw HttpException("Too Many Requests", response);
        case 500:
            throw HttpException("Internal Server Error", response);
        case 503:
            throw HttpException("Service Unavailable", response);
        case 508:
            throw HttpException("Loop Detected", response);
    }
    
    return HttpResponse(status, response_headers_map, response_body);
}

HttpRequest::~HttpRequest(void) {
    close(sock_fd);
}

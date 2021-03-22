//
//  url_parser.cpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/13/21.
//
#define assertm(exp, msg) assert(((void)msg, exp))

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include "url_parser.hpp"

UrlParser::UrlParser(string url) {
    assertm(!url.empty(), "url is not set");
    
    const char* url_c_str = url.c_str();
    
    // Protocol
    protocol = strtok(strcpy((char*) malloc(strlen(url_c_str) + 1), url_c_str), "://");
    
    // Hostname
    hostname = strstr(url_c_str, "://");
    if (hostname) {
        hostname += 3;
        char* site_port_path = strcpy((char*) calloc(1, strlen(hostname) + 1), hostname);
        hostname = strtok(site_port_path, ":");
        hostname = strtok(site_port_path, "/");
    } else {
        char* site_port_path = strcpy((char*) calloc(1, strlen(url_c_str) + 1), url_c_str);
        hostname = strtok(site_port_path, ":");
        hostname = strtok(site_port_path, "/");
    }
    
    // Port
    port = strstr(url_c_str + 6, ":");
    char* port_path = 0;
    char* port_path_cpy = 0;
    if (port && isdigit(*(port_path = (char*) port+1))) {
        port_path_cpy = strcpy((char*) malloc(strlen(port_path) + 1), port_path);
        char* r = strtok(port_path, "/");
        if (r) port = r;
        else port = port_path;
    } else port = "80";

    // Identifier
    if (port_path_cpy) identifier = port_path_cpy + strlen(port ? port : "");
    else {
        const char* identifier_tmp = strstr(url_c_str + 8, "/");
        identifier = identifier_tmp ? strcpy((char*) calloc(1, strlen(identifier_tmp) + 1), identifier_tmp) : "";
    }
    bool r = strcmp(protocol, hostname) == 0;
    if (r && strcmp(port, "80")) protocol = "http";
    else if (r) protocol = "tcp";
}

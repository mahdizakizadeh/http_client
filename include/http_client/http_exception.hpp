//
//  http_exception.hpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/12/21.
//

#ifndef HTTP_EXCEPTION_HPP_
#define HTTP_EXCEPTION_HPP_

#include <stdio.h>
#include <string>
#include <exception>
#include "http_response.hpp"

using std::exception;
using std::string;

class HttpException : public std::exception {
    
public:
    const HttpResponse httpResponse;
    const string message;
    
    HttpException(const string& message, const HttpResponse& httpResponse);
    
    virtual const char* what() const noexcept;
};

#endif /* http_exception_hpp */

//
//  http_exception.cpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/12/21.
//

#include "http_exception.hpp"

HttpException::HttpException(const string& message,
                             const HttpResponse& httpResponse)
    : httpResponse(httpResponse),
      message(message) { }

const char* HttpException::what() const noexcept {
    return message.c_str();
}

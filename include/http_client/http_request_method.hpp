//
//  http_request_method.hpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/13/21.
//

#ifndef HTTP_REQUEST_METHOD_HPP_
#define HTTP_REQUEST_METHOD_HPP_

using std::string;

enum HttpRequestMethod {
    kGet        = 0,
    kPost       = 1,
    kPut        = 2,
    kOptions    = 3,
    kHead       = 4,
    kDelete     = 5
};

#endif /* http_request_method_hpp */

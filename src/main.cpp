//
//  main.cpp
//  http
//
//  Created by Mahdi Zakizadeh on 3/12/21.
//

#include <iostream>
#include "http_request.hpp"

int main() {
    HttpRequest req { HttpRequest("https://google.com") };
    HttpResponse res = req.request();
    for (auto& item : res.headers) {
        std::cout << item.first << " : " << item.second << std::endl;
    }
    return 0;
}

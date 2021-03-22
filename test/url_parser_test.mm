//
//  url_parser_test.m
//  http_client_test
//
//  Created by Mahdi Zakizadeh on 3/22/21.
//

#import <XCTest/XCTest.h>

#include "url_parser.hpp"

@interface url_parser_test : XCTestCase

@end

@implementation url_parser_test

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testSimpleUrl {
    UrlParser parser = UrlParser("http://google.com/");
    printf("%s", parser.protocol);
    XCTAssertEqual(string("http"), parser.protocol);
    XCTAssertEqual(string("google.com"), parser.hostname);
    XCTAssertEqual(string("/"), parser.identifier);
    XCTAssertEqual(string("80"), parser.port);
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end

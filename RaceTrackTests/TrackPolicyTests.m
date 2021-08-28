//
//  TrackPolicyTests.m
//  RaceTrackTests
//
//  Created by Xiaoyue Ding on 8/26/21.
//

#import <XCTest/XCTest.h>
#import "TrackPolicy.hpp"

@interface TrackPolicyTests : XCTestCase {
@private
    TrackPolicy* testPolicy;
}

@end

@implementation TrackPolicyTests

- (void)setUp {
    testPolicy = new TrackPolicy();
}

- (void)tearDown {
    delete testPolicy;
    testPolicy = nullptr;
}

- (void)testUpdateGetStateActionVal {
    state_tuple testState_1 {std::make_tuple(0, 10, 0, 0)};
    std::tuple<int, int> testAcc_1 {std::make_tuple(1, 1)};
    
    testPolicy->updateStateActionVal(testState_1, testAcc_1, -1, 1);
    XCTAssertEqual(-1, testPolicy->getStateActionVal(testState_1, testAcc_1));
    
    testPolicy->updateStateActionVal(testState_1, testAcc_1, -1.5, 1);
    XCTAssertEqual(-1.25, testPolicy->getStateActionVal(testState_1, testAcc_1));
}

@end

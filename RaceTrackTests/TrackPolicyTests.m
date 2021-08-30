//
//  TrackPolicyTests.m
//  RaceTrackTests
//
//  Created by Xiaoyue Ding on 8/26/21.
//

#import <XCTest/XCTest.h>
#import "TrackPolicy.hpp"
#import "TrackFunctions.hpp"

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

- (void)testSetGetBehaveEpsilon {
    double testEpsilon {0.66};
    testPolicy->setBehaveEpsilon(testEpsilon);
    XCTAssertEqual(testEpsilon, testPolicy->getBehaveEpsilon());
}

- (void)testGetBehavePolicyProb {
    state_tuple testState_2 {std::make_tuple(1, 10, 4, 0)};
    std::tuple<int, int> testAcc_2;
    double testAccProb;
    
    std::vector<std::tuple<int, int>> allAvailAction;
    allAvailAction = actionSpace(testState_2);
    
    testAcc_2 = testPolicy->getBehavePolicy(testState_2);
    XCTAssertTrue(std::find(allAvailAction.begin(), allAvailAction.end(), testAcc_2) != allAvailAction.end());
    testAccProb = testPolicy->getBehaveProb(testState_2, testAcc_2);
    XCTAssertEqual(1.0, testAccProb * static_cast<int>(allAvailAction.size()));
}

- (void)testGetTargetPolicy {
    state_tuple testState_3 {std::make_tuple(1, 10, 4, 0)};
    std::tuple<int, int> testAcc_3, testAcc_4;
    
    testAcc_3 = std::make_tuple(-1, 1);
    testPolicy->updateStateActionVal(testState_3, testAcc_3, -1, 1);
    XCTAssertEqual(testAcc_3, testPolicy->getTargetPolicy(testState_3));
    
    testAcc_4 = std::make_tuple(0, 1);
    testPolicy->updateStateActionVal(testState_3, testAcc_4, -1.1, 1);
    XCTAssertEqual(testAcc_3, testPolicy->getTargetPolicy(testState_3));
    
    testPolicy->updateStateActionVal(testState_3, testAcc_3, -1.5, 1);
    XCTAssertEqual(testAcc_4, testPolicy->getTargetPolicy(testState_3));
}

@end

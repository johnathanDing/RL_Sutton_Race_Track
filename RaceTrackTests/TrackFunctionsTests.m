//
//  TrackFunctionsTests.m
//  RaceTrackTests
//
//  Created by Xiaoyue Ding on 8/18/21.
//

#import <XCTest/XCTest.h>
#import "TrackData.hpp"
#import "TrackEnv.hpp"
#import "TrackFunctions.hpp"
#import <algorithm>
#import <vector>


@interface TrackFunctionsTests : XCTestCase
{
    
}

@end

@implementation TrackFunctionsTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testActionSpace {
    state_tuple testCarState;
    std::vector<std::tuple<int, int>> testAction;
    
    testCarState = std::make_tuple(0, 0, 0, 0);
    testAction = actionSpace(testCarState);
    
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(1,0)) != testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(0,1)) != testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(1,1)) != testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(0,0)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(-1,-1)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(-1,0)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(-1,1)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(0,-1)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(1,-1)) == testAction.end());
    
    testCarState = std::make_tuple(0, 0, 4, 4);
    testAction = actionSpace(testCarState);
    
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(1,0)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(0,1)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(1,1)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(0,0)) != testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(-1,-1)) != testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(-1,0)) != testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(-1,1)) == testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(0,-1)) != testAction.end());
    XCTAssertTrue(std::find(testAction.begin(), testAction.end(), std::make_tuple(1,-1)) == testAction.end());
}


@end

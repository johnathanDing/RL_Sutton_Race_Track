//
//  TrackFunctionsTests.m
//  RaceTrackTests
//
//  Created by Xiaoyue Ding on 8/18/21.
//

#import <XCTest/XCTest.h>
#import "TrackFunctions.hpp"
#import "TrackData.hpp"
#import "TrackEnv.hpp"
#import <algorithm>
#import <vector>


@interface TrackFunctionsTests : XCTestCase

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
    
    testCarState = std::make_tuple(0, 0, 0, 0);
    testAction = actionSpace(testCarState);
    XCTAssertTrue(std::find(testAction.begin(), <#_InputIterator __last#>, <#const _Tp &__value_#>));
}


@end

//
//  TrackFunctionsTests.m
//  RaceTrackTests
//
//  Created by Xiaoyue Ding on 8/18/21.
//

#import <XCTest/XCTest.h>
#import "TrackData.hpp"
#import "TrackEnv.hpp"
#import "TrackPolicy.hpp"
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

- (void)testGenerateBehaveEpisode {
    int testTrackSize (30);
    TrackData testTrack {testTrackSize};
    TrackEnv testEnv {testTrack};
    TrackPolicy testPolicy;
    std::vector<state_action_reward_prob> testEpisode {generateBehaveEpisode(testEnv, testPolicy)};
    // Test the first step
    state_action_reward_prob first {testEpisode[0]};
    // Get the first state
    std::vector<std::tuple<int, int>> allAvailableAcc;
    allAvailableAcc = actionSpace(std::get<0>(first));
    
    XCTAssertEqual(0, std::get<0>(std::get<0>(first)));
    XCTAssertTrue(std::get<0>(std::get<1>(first))>0 || std::get<1>(std::get<1>(first))>0);
    XCTAssertEqual(-1, std::get<2>(first));
    XCTAssertEqual(1.0/static_cast<int>(allAvailableAcc.size()), std::get<3>(first));
}

@end

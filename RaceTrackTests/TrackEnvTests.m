//
//  TrackEnvTests.m
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/17/21.
//

#import <XCTest/XCTest.h>
#import "TrackData.hpp"
#import "TrackEnv.hpp"
#import <iostream>

@interface TrackEnvTests : XCTestCase {
@private
    int testTrackSize;
    TrackData* testTrack;
    TrackEnv* testEnv;
    vector_2D raceTrack;
    std::vector<std::tuple<int, int>> startLine;
    std::vector<std::tuple<int, int>> finishLine;
};

@end

@implementation TrackEnvTests

- (void)setUp {
    testTrackSize = 40;
    testTrack = new TrackData(testTrackSize);
    testEnv = new TrackEnv(*testTrack);
    raceTrack = testTrack->getRaceTrack();
    for (int i_col{0}; i_col < testTrackSize; ++i_col) {
        if (raceTrack[0][i_col] == 1) {
            startLine.push_back(std::make_tuple(0, i_col));
        }
    }
    for (int i_row{0}; i_row < testTrackSize; ++i_row) {
        if (raceTrack[i_row][testTrackSize-1] == 1) {
            finishLine.push_back(std::make_tuple(i_row, testTrackSize-1));
        }
    }
}

- (void)tearDown {
    delete testEnv;
    delete testTrack;
    testEnv = nullptr;
    testTrack = nullptr;
}

- (void)testGetStartState {
    state_tuple testStartState {testEnv->getStartState()};
    // Start state should be row 0
    XCTAssertEqual(0, std::get<0>(testStartState));
    // Start state should be in range of start line
    XCTAssertTrue((std::get<1>(testStartState)>=std::get<1>(startLine[0])));
    XCTAssertTrue((std::get<1>(testStartState)<=std::get<1>(startLine.back())));
    // Start state should have velocity set to zero
    XCTAssertEqual(0, std::get<2>(testStartState));
    XCTAssertEqual(0, std::get<3>(testStartState));
}

- (void)testGetEnvRes_startLine {
    std::tuple<int, int> startLineMid {startLine[startLine.size()/2]};
    state_tuple testCurrState;
    std::tuple<int, int> testAcceleration;
    
    // First test starting line
    testCurrState = std::make_tuple(std::get<0>(startLineMid), std::get<1>(startLineMid), 1, 1);
    testAcceleration = std::make_tuple(1, 1);
    
    envResponse testNextState {testEnv->getEnvResponse(testCurrState, testAcceleration)};
    // Step reward is always -1
    XCTAssertEqual(-1, testNextState.reward);
    // Position should move according to velocity
    XCTAssertEqual(std::get<0>(startLineMid)+1, std::get<0>(testNextState.nextState));
    XCTAssertEqual(std::get<1>(startLineMid)+1, std::get<1>(testNextState.nextState));
    // Velocity should change according to velocity
    XCTAssertEqual(2, std::get<2>(testNextState.nextState));
    XCTAssertEqual(2, std::get<3>(testNextState.nextState));
    // No way the finish line is crossed when starting at start line
    XCTAssertFalse(testNextState.finished);
}

- (void)testGetEnvRes_finishLine {
    std::tuple<int, int> finishLineBot {finishLine.back()};
    state_tuple testCurrState;
    std::tuple<int, int> testAcceleration;
    
    // Then test finish line.
    // Choose a position close to finish line with trajectory certain to cross it.
    testCurrState = std::make_tuple(std::get<0>(finishLineBot)-1, std::get<1>(finishLineBot)-1, 2, 2);
    testAcceleration = std::make_tuple(1, 1);
    
    envResponse testNextState {testEnv->getEnvResponse(testCurrState, testAcceleration)};
    // Step reward is always -1
    XCTAssertEqual(-1, testNextState.reward);
    // Finish line crossed. Should be reset to random start state
    XCTAssertEqual(testTrackSize-1, std::get<1>(testNextState.nextState));
    XCTAssertTrue((std::get<0>(testNextState.nextState)>=std::get<0>(finishLine[0])));
    XCTAssertTrue((std::get<0>(testNextState.nextState)<=std::get<0>(finishLine.back())));
    // Finish line crossed. Velocity should be reset
    XCTAssertEqual(0, std::get<2>(testNextState.nextState));
    XCTAssertEqual(0, std::get<3>(testNextState.nextState));
    // Finish line crossed.
    XCTAssertTrue(testNextState.finished);
}

- (void)testGetEnvRes_OOTrack {
    std::tuple<int, int> startLineLast {startLine.back()};
    state_tuple testCurrState;
    std::tuple<int, int> testAcceleration;
    
    // First test starting line
    testCurrState = std::make_tuple(std::get<0>(startLineLast), std::get<1>(startLineLast)-1, 0, 2);
    testAcceleration = std::make_tuple(1, 1);
    
    envResponse testNextState {testEnv->getEnvResponse(testCurrState, testAcceleration)};
    // Step reward is always -1
    XCTAssertEqual(-1, testNextState.reward);
    // Out of track. Should be reset to random start state
    XCTAssertEqual(0, std::get<0>(testNextState.nextState));
    XCTAssertTrue((std::get<1>(testNextState.nextState)>=std::get<1>(startLine[0])));
    XCTAssertTrue((std::get<1>(testNextState.nextState)<=std::get<1>(startLine.back())));
    // Out of track. Velocity should be reset
    XCTAssertEqual(0, std::get<2>(testNextState.nextState));
    XCTAssertEqual(0, std::get<3>(testNextState.nextState));
    // No way the finish line is crossed when starting at start line
    XCTAssertFalse(testNextState.finished);
}

@end

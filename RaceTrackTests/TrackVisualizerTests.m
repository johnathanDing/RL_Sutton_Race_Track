//
//  TrackVisualizerTests.m
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/13/21.
//

#import <XCTest/XCTest.h>
#import "TrackVisualizer.hpp"
#import <iostream>

@interface TrackVisualizerTests : XCTestCase {
//@private
//    int testTrackSize;
//    TrackVisualizer* testVisualizer;
}

@end

/// SFML required building settings incompatible with Xcode unit testing
@implementation TrackVisualizerTests

- (void)setUp {
//    testTrackSize = 40;
//    testVisualizer = new TrackVisualizer(TrackData(testTrackSize));
    std::cout << "Track visualizer tester disabled due to SFML not compatible with Xcode unit testing." << "\n";
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

@end

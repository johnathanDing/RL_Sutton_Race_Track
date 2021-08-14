//
//  TrackDataTests.m
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/13/21.
//

#import <XCTest/XCTest.h>
#import "TrackData.hpp"
#import <iostream>

@interface TrackDataTests : XCTestCase {
@private
    int testTrackSize;
    // TrackData pointer to store instance during setUp
    TrackData* testTrack;
};

@end

@implementation TrackDataTests

- (void)setUp {
    testTrackSize = 40;
    testTrack = new TrackData(testTrackSize);
}

- (void)tearDown {
    // Releases the allocated TrackData instance
    delete testTrack;
    testTrack = nullptr;
}

- (void)testGetTrackSize {
    XCTAssertEqual(testTrackSize, testTrack->getTrackSize());
}

- (void)testGetRaceTrack {
    // Get the generated race track
    const vector_2D& trackResult = testTrack->getRaceTrack();
    // Print out the starting line for visual check
    for (int i{0}; i < testTrackSize; ++i) {
        std::cout << trackResult[0][i] << " ";
    }
    std::cout << "\n";
    // Both race track dimension should match track size
    XCTAssertEqual(testTrackSize, trackResult.size());
    XCTAssertEqual(testTrackSize, trackResult[0].size());
}

@end

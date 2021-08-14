//
//  main.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/1/21.
//

#include "TrackData.hpp"
#include "TrackVisualizer.hpp"
#include <iostream>

int main() {
    
    int trackSize {40};
    TrackData testTrack {trackSize};
    TrackVisualizer testVisual {testTrack};
    
    tuple_list testCarTraj;
    for (int i{0}; i < trackSize; ++i) {
        testCarTraj.push_back(std::make_tuple(0, i, 0, 0));
    }
    
    testVisual.drawRaceTrack(testCarTraj);
    
//    std::cout << testTrack.getTrackSize() << '\n';
    
    return 0;
}


//
//  main.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/1/21.
//

#include "TrackData.hpp"
#include "TrackVisualizer.hpp"
#include "TrackEnv.hpp"
#include "TrackFunctions.hpp"
#include <iostream>

int main() {
    
    int trackSize {50};
    TrackData testTrack {trackSize};
    TrackVisualizer testVisual {testTrack};
    
    tuple_list testCarTraj;
    for (int i{0}; i < trackSize; ++i) {
        testCarTraj.push_back(std::make_tuple(0, i, 0, 0));
    }
    
//    testVisual.drawRaceTrack(testCarTraj);
    
    state_tuple testState {std::make_tuple(0, 0, 0, 0)};
    
    for (std::tuple<int, int> acc : actionSpace(testState)) {
        std::cout << "(" << std::get<0>(acc) << ", " << std::get<1>(acc) << "), ";
    }
    std::cout<< "\n";
    
//    std::cout << testTrack.getTrackSize() << '\n';
    
    return 0;
}


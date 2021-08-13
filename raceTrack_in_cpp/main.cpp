//
//  main.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/1/21.
//

#include "TrackData.hpp"
#include <iostream>

int main() {
    
    TrackData testTrack {40};
    
    std::cout << testTrack.getTrackSize() << '\n';
    
    return 0;
}


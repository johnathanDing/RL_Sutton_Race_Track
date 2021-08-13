//
//  TrackVisualizer.hpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/13/21.
//

#ifndef TrackVisualizer_hpp
#define TrackVisualizer_hpp

#include "TrackData.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>

class TrackVisualizer
{
private:
    int trackSize;
    // A constant reference to given race track
    const vector_2D& raceTrack;
    bool hasWindow;
    
public:
    // Constructor takes in race track info from TrackData
    TrackVisualizer(const TrackData& inputTrack);
    // Public method for drawing race track given car state array
    void drawRaceTrack(std::vector<std::tuple<int, int, int, int>> carState);
};

#endif /* TrackVisualizer_hpp */

//
//  TrackData.hpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/9/21.
//

#ifndef TrackData_hpp
#define TrackData_hpp

//#include <stdio.h>
#include <vector>
#include <random>
#include <time.h>
#include <iostream>

using rand_uni_int = std::uniform_int_distribution<>;
using rand_choices = std::discrete_distribution<>;
using vector_2D = std::vector<std::vector<int>>;

/// Class that generates and stores race track
class TrackData
{
private:
    // Edge length of square-shaped race track
    const int trackSize;
    // 2D vector of race track
    vector_2D raceTrack;
    
public:
    /// TrackData class constructor
    /// @param trackSizeInput How many squares do you want the race track edge to be? Default is 30.
    TrackData(int trackSizeInput = 30);
    
    /// Returns track edge size
    int getTrackSize() const;
    
    /// Returns race track as a 2D vector constant reference
    const vector_2D& getRaceTrack() const;
    
private:
    /// Generate the race track according to author-defined rules
    void generateRaceTrack();
};

#endif /* TrackData_hpp */

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
    // Constructor. Edge size of 30 is a good default value
    TrackData(int trackSizeInput = 30);
    // To give public access to track size
    int getTrackSize() const;
    // To give public access to a const reference of race track
    const vector_2D& getRaceTrack() const;
    
private:
    // Generates race track given track size
    void generateRaceTrack();
};

#endif /* TrackData_hpp */

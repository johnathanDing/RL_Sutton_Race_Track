//
//  TrackData.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/9/21.
//

#include "TrackData.hpp"
#include <vector>
#include <random>
#include <time.h>

TrackData::TrackData(int trackSizeInput) :
    trackSize{trackSizeInput},
    raceTrack(trackSizeInput, std::vector<int>(trackSizeInput, 0))
{
    generateRaceTrack();
};
    

int TrackData::getTrackSize() const
{
    return trackSize;
};

const vector_2D& TrackData::getRackTrack() const
{
    return raceTrack;
};
    
void TrackData::generateRaceTrack()
{
    // Define the min/max starting line width
    int minLineWidth {trackSize/6};
    int maxLineWidth {trackSize/2};
    
    // System clock Mersenne engine to avoid repeated race tracks
    std::mt19937 mersenneEng
    {static_cast<std::mt19937::result_type>(std::time(nullptr))};
    
    // Make turning point a bit past half way
    rand_uni_int turningPntRNG
    {trackSize/2, trackSize*2/3};
    // Make start line width between min/max width above
    rand_uni_int startLineRNG
    {minLineWidth, maxLineWidth};
    
    // Race track turning point towards finish line
    int turningPoint {turningPntRNG(mersenneEng)};
    
    // Initilize random start line
    int startLineWidth {startLineRNG(mersenneEng)};
    int startLineLeftLim {trackSize-1+startLineWidth*3/2};
    int startLineLeft {0};
    if (startLineLeftLim > 0) {
        rand_uni_int limRNG {0, startLineLeftLim};
        startLineLeft = limRNG(mersenneEng);
    }
    int startLineRight {startLineLeft + startLineWidth - 1};
    
    // Initialize the starting line in race track
    for (int i{startLineLeft}; i <= startLineRight; ++i) {
        raceTrack[0][i] = 1;
    }
    
    // Going upwards, race track random walks until turning point
    // Once past that, track quickly goes towards finish line
    // Current boundary from starting line
    int rowLeft {startLineLeft};
    int rowRight {startLineRight};
    // Weighted RNG
    rand_choices left_before {1, 1, 1};
    rand_choices left_after {1, 3, 3};
    
    
    // Possible next move of track left boundary
    int nextMoveLeft[3] {-1, 0, 1};
    
    for (int i_row{1}; i_row < trackSize; ++i_row) {
        // Move
        if (i_row < turningPoint) {
            rowLeft = std::max(0, rowLeft
                               + nextMoveLeft[left_before(mersenneEng)]);
        } else {
            rowLeft = std::max(0, rowLeft
                               + nextMoveLeft[left_after(mersenneEng)]);
        }
    }
    
    return;
};

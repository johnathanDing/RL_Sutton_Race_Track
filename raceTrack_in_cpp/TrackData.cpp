//
//  TrackData.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/9/21.
//

#include "TrackData.hpp"
#include <time.h>
#include <iostream>
//#include <vector>
//#include <random>


/// TrackData class constructor
/// @param trackSizeInput How many squares do you want the race track edge to be? Default is 30.
TrackData::TrackData(int trackSizeInput) :
    trackSize{trackSizeInput},
    raceTrack(trackSizeInput, std::vector<int>(trackSizeInput, 0))
{
    generateRaceTrack();
    std::cout << "Race track generated..." << "\n";
};
    

/// Returns track edge size
int TrackData::getTrackSize() const
{
    return trackSize;
};

/// Returns race track as a 2D vector constant reference
const vector_2D& TrackData::getRaceTrack() const
{
    return raceTrack;
};

/// Generate the race track according to author-defined rules
void TrackData::generateRaceTrack()
{
    // Define the min/max starting line width
    int minLineWidth {trackSize/6};
    int maxLineWidth {trackSize/3};
    
    // System clock Mersenne engine to avoid repeated race tracks
    std::mt19937 mersenneEng
    {static_cast<std::mt19937::result_type>(std::time(nullptr))};
    
    // Make turning point a bit past half way
    rand_uni_int turningPntRNG
    {trackSize*2/3, trackSize*5/6};
    // Make start line width between min/max width above
    rand_uni_int startLineRNG
    {minLineWidth, maxLineWidth};
    
    // Race track turning point towards finish line
    int turningPoint {turningPntRNG(mersenneEng)};
    
    // Initilize random start line
    int startLineWidth {startLineRNG(mersenneEng)};
    int startLineLeftLim {trackSize-1-startLineWidth*2};
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
    rand_choices leftBefore {1, 1, 1}; // left bound before turning point
    rand_choices leftAfter {1, 3, 3};  // left bound after turning point
    rand_choices rightBefore {3, 1};   // right bound before turning point
    rand_choices rightAfter {1, 2, 1}; // right bound after turning point
    
    // Possible next moves
    int nextMoveLeft[3] {-1, 0, 1};     // left bound
    int nextMoveRightBefore[2] {0, 1};  // right bound before turning point
    int nextMoveRightAfter[3] {0,1,trackSize-1-rowRight}; // right bound after turning point
    
    for (int i_row{1}; i_row < trackSize; ++i_row) {
        // Move left bound according to above rules
        if (i_row < turningPoint) {
            rowLeft += nextMoveLeft[leftBefore(mersenneEng)];
        } else {
            rowLeft += nextMoveLeft[leftAfter(mersenneEng)];
        }
        rowLeft = std::max(0, rowLeft);
        
        // Move right bound according to above rules
        if (rowRight < trackSize-1) {
            if (i_row < turningPoint) {
                rowRight += nextMoveRightBefore[rightBefore(mersenneEng)];
            } else {
                rowRight += nextMoveRightAfter[rightAfter(mersenneEng)];
            }
        }
        rowRight = std::min(trackSize-1, rowRight);
        
        for (int i_col{rowLeft}; i_col <= rowRight; ++i_col) {
            raceTrack[i_row][i_col] = 1;
        }
        
    }
    
    return;
};

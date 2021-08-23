//
//  TrackEnv.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/14/21.
//

#include "TrackEnv.hpp"


TrackEnv::TrackEnv(const TrackData& inputTrack):
trackSize(inputTrack.getTrackSize()),
raceTrack(inputTrack.getRaceTrack())
{
    // Get the track start line
    for (int i_col{0}; i_col < trackSize; ++i_col) {
        if (raceTrack[0][i_col] == 1) {
            startLine.push_back(std::make_tuple(0, i_col));
        }
    }
    // Get the track finish line
    for (int i_row{0}; i_row < trackSize; ++i_row) {
        if (raceTrack[i_row][trackSize-1] == 1) {
            finishLine.push_back(std::make_tuple(i_row, trackSize-1));
        }
    }
};


state_tuple TrackEnv::getStartState()
{
    // System clock Mersenne engine
    std::mt19937 mersenneEng
    {static_cast<std::mt19937::result_type>(std::time(nullptr))};
    // A uniform integer RNG to randomly choose a starting position
    rand_uni_int startLineRNG {0, static_cast<int>(startLine.size())-1};
    // Get a random starting position
    std::tuple<int, int> startPos {startLine[startLineRNG(mersenneEng)]};
    // Add double zero velocity to the state tuple
    return std::make_tuple(std::get<0>(startPos), std::get<1>(startPos), 0, 0);
};


envResponse TrackEnv::getEnvResponse
 (state_tuple currState, std::tuple<int, int> acceleration)
{
    // Initialize a next response struct
    envResponse nextResp;
    
    // Get current position for convenience
    int currPos_i {std::get<0>(currState)};
    int currPos_j {std::get<1>(currState)};
    
    // Get next position first
    int nextPos_i {currPos_i+std::get<2>(currState)};
    int nextPos_j {currPos_j+std::get<3>(currState)};
    
    // If the next position is not on track
    if (nextPos_i>=trackSize-1 || nextPos_j >= trackSize-1
        || raceTrack[nextPos_i][nextPos_j] != 1)
    {
        // See if the next position crosses the finish line
        if (std::get<1>(currState)<trackSize-1 && nextPos_j>=trackSize-1
         && std::min(currPos_i, nextPos_i) <= std::get<0>(finishLine.back())
         && std::max(currPos_i, nextPos_i) >= std::get<0>(finishLine[0]))
        {
            nextResp.reward = -1;
            // Next position hard to represent here. Just jump to a random start
            nextResp.nextState = getStartState();
            nextResp.finished = true;
        }
        // Or just plain out of track. Restart
        else {
            nextResp.reward = -1;
            nextResp.nextState = getStartState();
            nextResp.finished = false;
        }
    }
    // If the next position is still on tracl
    else {
        // See if the next position is right on the finish line
        if (nextPos_j == trackSize-1) {
            nextResp.reward = -1;
            nextResp.nextState = std::make_tuple(nextPos_i, nextPos_j, 0, 0);
            nextResp.finished = true;
        }
        // Or just anywhere else on the track.
        // This is the only case where we need to consider the next velocity
        else {
            // Get the next velocity
            int nextVelo_i {std::get<2>(currState) + std::get<0>(acceleration)};
            int nextVelo_j {std::get<3>(currState) + std::get<1>(acceleration)};
            // Assert the velocity constrains.
            // Ideally, this should have been taken care of by the policy.
            assert((nextVelo_i>=0 && nextVelo_j>=0) && "Negative velocity encountered.");
            assert((nextVelo_i<5 && nextVelo_j<5) && "Larger-than-5 velocity encountered.");
            assert(!(nextVelo_i==0 || nextVelo_j==0)
                   && "All zero velocity encountered at non-start line.");
            // If all assertions passed, the next velocity is valid
            nextResp.reward = -1;
            nextResp.nextState = std::make_tuple(nextPos_i, nextPos_j, nextVelo_i, nextVelo_j);
            nextResp.finished = false;
        }
    }
    
    return nextResp;
};

//
//  TrackEnv.hpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/14/21.
//

#ifndef TrackEnv_hpp
#define TrackEnv_hpp

#include "TrackData.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <cassert>

using state_tuple = std::tuple<int, int, int, int>;

/// Struct storing env response including reward, next state, and whether the finish line has been crossed
struct envResponse
{
    // Reward to the step
    int reward;
    // The next state(four-) tuple
    state_tuple nextState;
    // Whether the finish line is crossed at this step
    bool finished;
};

/// Class that simulates the track environment
class TrackEnv
{
private:
    int trackSize;
    // A constant reference to given race track
    const vector_2D& raceTrack;
    // Easy access to starting line positions
    std::vector<std::tuple<int, int>> startLine;
    // Easy access to finish line positions
    std::vector<std::tuple<int, int>> finishLine;
    
public:
    // Constuctor based on input TrackData
    TrackEnv(const TrackData& inputTrack);
    // Get a starting state
    state_tuple getStartState();
    // Get response from environment the response struct, based on current state and acceleration
    envResponse getEnvResponse(state_tuple currState, std::tuple<int, int> acceleration);
};

#endif /* TrackEnv_hpp */

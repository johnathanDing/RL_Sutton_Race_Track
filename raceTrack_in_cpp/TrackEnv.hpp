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
    /// Construct track parameters based on input TrackData
    /// @param inputTrack Input TrackData class instance
    TrackEnv(const TrackData& inputTrack);
    
    /// Returns a random starting state as state (4-) tuple
    state_tuple getStartState();
    
    /// Given current state and action (acceleration), return the next environment response struct
    /// @param currState Current state tuple
    /// @param acceleration Chosen action
    envResponse getEnvResponse(state_tuple currState, std::tuple<int, int> acceleration);
};

#endif /* TrackEnv_hpp */

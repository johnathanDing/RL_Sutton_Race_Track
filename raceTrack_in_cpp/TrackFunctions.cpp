//
//  TrackFunctions.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/17/21.
//

#include "TrackFunctions.hpp"

std::vector<std::tuple<int, int>> actionSpace(state_tuple carState)
{
    // Full list of available actions (acceleration)
    std::vector<std::tuple<int, int>> actionFullSpace
    {
        std::make_tuple(-1, -1),
        std::make_tuple(-1, 0),
        std::make_tuple(-1, 1),
        std::make_tuple(0, -1),
        std::make_tuple(0, 0),
        std::make_tuple(0, 1),
        std::make_tuple(1, -1),
        std::make_tuple(1, 0),
        std::make_tuple(1, 1)
    };
    // To store final list of available actions
    std::vector<std::tuple<int, int>> actionAvailable;
    // As in name
    int currVelocity_i {std::get<2>(carState)};
    int currVelocity_j {std::get<3>(carState)};
    // As in name
    int nextVelocity_i;
    int nextVelocity_j;
    // Conditions to be met for the action to qualify
    bool velocityInRange;
    bool velocityNonZero;
    // Iterate through full action space
    for (std::tuple<int, int> acc : actionFullSpace) {
        // Get values of next velocity
        nextVelocity_i = currVelocity_i + std::get<0>(acc);
        nextVelocity_j = currVelocity_j + std::get<1>(acc);
        // Check if conditions are met
        velocityInRange = (nextVelocity_i>=0) && (nextVelocity_i<5)
                       && (nextVelocity_j>=0) && (nextVelocity_j<5);
        velocityNonZero = !((nextVelocity_i==0) && (nextVelocity_j==0));
        // If meet conditions, add to available action list
        if (velocityInRange && velocityNonZero) {
            actionAvailable.push_back(acc);
        }
    }
    
    return actionAvailable;
    
};


std::vector<state_action_reward> generateBehaveEpisode(const TrackEnv& inputTrackEnv, const TrackPolicy& inputTrackPolicy)
{
    // Initialize a full episode
    std::vector<state_action_reward> episode;
    // Get the starting state, action, and next response
    state_tuple currState {inputTrackEnv.getStartState()};
    std::tuple<int, int> currAction {inputTrackPolicy.getBehavePolicy(currState)};
    envResponse currNext {inputTrackEnv.getEnvResponse(currState, currAction)};
    // As long as episode is not finished (finish line not crossed) yet
    while (!currNext.finished) {
        // Add current state-action pair into episode
        episode.push_back(std::make_tuple(currState, currAction, currNext.reward));
        // Get next state-action pair
        currState = currNext.nextState;
        currAction = inputTrackPolicy.getBehavePolicy(currState);
        // Calculate next environment response
        currNext = inputTrackEnv.getEnvResponse(currState, currAction);
    }
    episode.push_back(std::make_tuple(currState, currAction, currNext.reward));
    
    return episode;
}


std::vector<state_action_reward> generateTargetEpisode(const TrackEnv& inputTrackEnv, const TrackPolicy& inputTrackPolicy)
{
    // Initialize a full episode
    std::vector<state_action_reward> episode;
    // Get the starting state, action, and next response
    state_tuple currState {inputTrackEnv.getStartState()};
    std::tuple<int, int> currAction {inputTrackPolicy.getTargetPolicy(currState)};
    envResponse currNext {inputTrackEnv.getEnvResponse(currState, currAction)};
    // As long as episode is not finished (finish line not crossed) yet
    while (!currNext.finished) {
        // Add current state-action pair into episode
        episode.push_back(std::make_tuple(currState, currAction, currNext.reward));
        // Get next state-action pair
        currState = currNext.nextState;
        currAction = inputTrackPolicy.getTargetPolicy(currState);
        // Calculate next environment response
        currNext = inputTrackEnv.getEnvResponse(currState, currAction);
    }
    episode.push_back(std::make_tuple(currState, currAction, currNext.reward));
    
    return episode;
}

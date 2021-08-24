//
//  TrackPolicy.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/21/21.
//

#include "TrackPolicy.hpp"

TrackPolicy::TrackPolicy(double epsilonInput):
epsilonSoft(epsilonInput)
{
    std::cout << "Track Policy initialized" << "\n";
};


void TrackPolicy::updateStateActionVal(state_tuple carState, std::tuple<int, int> acc,
                                       double newReturn, double newWeight)
{
    // If never seen this state
    if (stateActionSpace.find(carState) == stateActionSpace.end()) {
        
        // Add car state and action into the state-action space map
        stateActionSpace.insert(std::pair<state_tuple, std::vector<std::tuple<int, int>>>
                                (carState, std::vector<std::tuple<int, int>>{{acc}}));
        
        // Add car state and weight into the state-action weight map
        // Indices corrspond to the state-action space map
        stateActionWeight.insert(std::pair<state_tuple, std::vector<double>>
                                 (carState, std::vector<double>{newWeight}));
        
        // Add car state and value into the state-action value map
        // Indices corrspond to the state-action space map
        stateActionValue.insert(std::pair<state_tuple, std::vector<double>>
                                (carState, std::vector<double>{newReturn}));
        
        // Initialize state visit count
        stateVisitCount.insert(std::pair<state_tuple, int> (carState, 1));
    }
    // If car state is already in map
    else {
        // Iterator for the action space
        std::vector<std::tuple<int, int>>::iterator iterAction;
        // Find the iterator to the action
        iterAction = std::find(stateActionSpace[carState].begin(),
                              stateActionSpace[carState].end(), acc);
        
        // If action have not been seen yet
        if (iterAction == stateActionSpace[carState].end()) {
            // Append the new action to state-action space map
            stateActionSpace[carState].push_back(acc);
            // Add car state and weight into the state-action weight map
            // Indices corrspond to the state-action space map
            stateActionWeight[carState].push_back(newWeight);
            // Add car state and value into the state-action value map
            // Indices corrspond to the state-action space map
            stateActionValue[carState].push_back(newReturn);
        }
        // If action is already in map
        else {
            // Find the index of the inquired action
            int idxAction = static_cast<int>(std::distance(stateActionSpace[carState].begin(), iterAction));
            // Update state-aciton weight
            stateActionWeight[carState][idxAction] += newWeight;
            // Store the current total weigth for convenience
            double currWeigth = stateActionWeight[carState][idxAction];
            // Get the current state-action value for convenience
            double currVal = stateActionValue[carState][idxAction];
            // Update state-aciton value
            stateActionValue[carState][idxAction] += newWeight/currWeigth * (newReturn - currVal);
        }
        
        // Update state visit count
        stateVisitCount[carState] += 1;
    }
};


double TrackPolicy::getStateActionVal(state_tuple carState, std::tuple<int, int> acc)
{
    // Iterator for the action space
    std::vector<std::tuple<int, int>>::iterator iterAction;
    // Find the iterator to the action
    iterAction = std::find(stateActionSpace[carState].begin(), stateActionSpace[carState].end(), acc);
    // Find the index of the inquired action
    int idxAction = static_cast<int>(std::distance(stateActionSpace[carState].begin(), iterAction));
    // Return state-action value according to the same index
    return stateActionValue[carState][idxAction];
};


void TrackPolicy::updateTargetPolicy(state_tuple carState)
{
    // If policy hasn't seen the state yet, initialize it
    if (targetPolicy.find(carState) == targetPolicy.end()) {
        targetPolicy.insert(std::pair<state_tuple, std::tuple<int, int>>
                            (carState, std::tuple<int, int> (0, 0)));
    }
    
    // Find argmax of state-action value
    std::vector<double>::iterator iterVal = std::max_element(stateActionValue[carState].begin(),
                                                             stateActionValue[carState].end());
    int idxMaxVal = static_cast<int>(std::distance(stateActionValue[carState].begin(), iterVal));
    // Update target policy for the inquired state
    targetPolicy[carState] = stateActionSpace[carState][idxMaxVal];
};


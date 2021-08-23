//
//  TrackPolicy.hpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/21/21.
//

#ifndef TrackPolicy_hpp
#define TrackPolicy_hpp

#include "TrackData.hpp"
#include "TrackEnv.hpp"
#include "TrackFunctions.hpp"
#include <vector>
#include <random>
#include <unordered_map>

using stateActionSpaceType = std::unordered_map<state_tuple, std::vector<std::tuple<int, int>>>;
using stateActionValueType = std::unordered_map<state_tuple, std::vector<double>>;
using targetPolicyType = std::unordered_map<state_tuple, std::tuple<int, int>>;

/// Class that stores state action (Q-) values, makes and modifies target and behavior policies
class TrackPolicy
{
private:
    // Stores the state-action space
    stateActionSpaceType stateActionSpace;
    // Stores the state-action value
    stateActionValueType stateActionValue;
    // A constantly updating target policy on the go
    targetPolicyType targetPolicy;
    // Soft parameter for the behavior policy
    double epsilonSoft;
public:
    /// Constructor for the TrackPolicy class
    /// @param epsilonInput Initial value for the behavior policy epsilon soft parameter. Default is 1.0, a uniformly random policy.
    TrackPolicy(double epsilonInput = 1.0);
    
    /// Updates the state-action (Q-) value according to new observed values. Returns void type.
    /// @param carState Car state to be updated
    /// @param acc Action (Acceleration) to be updated
    /// @param newVal Newly observed state-action value
    void updateStateActionVal(state_tuple carState, std::tuple<int, int> acc, double newVal);
    
    /// Returns the current state-aciton value.
    /// @param carState Car state to be inquired
    /// @param acc Action (Acceleration) to be inquired
    double getStateActionVal(state_tuple carState, std::tuple<int, int> acc);
    
    /// Returns the action under the current target policy for a specific state
    /// @param carState Car state to be inquired
    std::tuple<int, int> getTargetPolicy(state_tuple carState);
    
    /// Sets the epsilon soft parameter of the behavior state
    /// @param epsilon Epsilon soft parameter
    void setBehaveEpsilon(double epsilon);
    
    /// Returns the current epsilon soft parameter of the behavior policy
    double getBehaveEpsilon();
    
    /// Returns the action and probability (importance sampling) under behavior policy for a specific state
    /// @param carState Car state to be inquired
    std::tuple<int, int, double> getBehavePolicy(state_tuple carState);
};

#endif /* TrackPolicy_hpp */

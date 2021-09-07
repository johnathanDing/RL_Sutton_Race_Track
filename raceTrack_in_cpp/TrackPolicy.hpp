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
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <math.h>

using stateActionSpaceType = std::map<state_tuple, std::vector<std::tuple<int, int>>>;
using stateActionValueType = std::map<state_tuple, std::vector<double>>;
using stateVisitCountType = std::map<state_tuple, int>;
using targetPolicyType = std::map<state_tuple, std::tuple<int, int>>;

/// Class that stores state action (Q-) values, makes and modifies target and behavior policies
class TrackPolicy
{
private:
    // Stores the state-action space
    stateActionSpaceType stateActionSpace;
    // Stores the state-action cumulative weight
    stateActionValueType stateActionWeight;
    // Stores the state-action value
    stateActionValueType stateActionValue;
    // Stores the state-action count of visits
    stateVisitCountType stateVisitCount;
    // A constantly updating target policy on the go
    targetPolicyType targetPolicy;
    // Soft parameter for the behavior policy
    double epsilonSoft;
    // Threshold for soft behavior policy to become greedy
    int stateVisitThreshold;
    
public:
    /// Constructor for the TrackPolicy class
    /// @param epsilonInput Initial value for the behavior policy epsilon soft parameter. Default is 1.0, a uniformly random policy.
    /// @param stateVisitThreshInput How many visits to a state before starting to converge behavior policy to target policy
    TrackPolicy(double epsilonInput = 1.0, int stateVisitThreshInput = 100);
    
    /// Updates the state-action (Q-) value according to new observed values. Returns void type.
    /// @param carState Car state to be updated
    /// @param acc Action (Acceleration) to be updated
    /// @param newReturn G, the new return value to be updated
    /// @param newWeight W, the new sampling weight to be updated
    void updateStateActionVal(state_tuple carState, std::tuple<int, int> acc,
                              double newReturn, double newWeight);
    
    /// Returns the current state-aciton value.
    /// @param carState Car state to be inquired
    /// @param acc Action (Acceleration) to be inquired
    double getStateActionVal(state_tuple carState, std::tuple<int, int> acc) const;
    
    /// Sets the epsilon soft parameter of the behavior state
    /// @param epsilon Epsilon soft parameter
    void setBehaveEpsilon(double epsilon);
    
    /// Returns the current epsilon soft parameter of the behavior policy
    double getBehaveEpsilon() const;
    
    /// Returns the action and probability (importance sampling) under behavior policy for a specific state
    /// @param carState Car state to be inquired
    std::tuple<int, int> getBehavePolicy(state_tuple carState) const;
    
    /// Get the probability of getting such action from behavior policy for importance sampling
    /// @param carState Car state inquired
    /// @param acc Action inquired
    double getBehaveProb(state_tuple carState, std::tuple<int, int> acc) const;
    
    /// Returns the action under the current target policy for a specific state
    /// @param carState Car state to be inquired
    std::tuple<int, int> getTargetPolicy(state_tuple carState) const;
    
private:
    /// Re-calculate and update the target policy
    /// @param carState Car state to be updated
    void updateTargetPolicy(state_tuple carState);
};

#endif /* TrackPolicy_hpp */

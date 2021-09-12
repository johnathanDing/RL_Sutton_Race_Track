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

std::vector<std::tuple<int, int>> actionSpace(state_tuple carState);

/// Class that stores state action (Q-) values, makes and modifies target and behavior policies
class TrackPolicy
{
private:
    // Stores the state-action space
    std::map<state_tuple, std::vector<std::tuple<int, int>>> stateActionSpace;
    // Stores the state-action cumulative weight
    std::map<state_tuple, std::vector<double>> stateActionWeight;
    // Stores the state-action value
    std::map<state_tuple, std::vector<double>> stateActionValue;
    // Stores the state-action count of visits
    std::map<state_tuple, int> stateVisitCount;
    // A constantly updating target policy on the go
//    targetPolicyType targetPolicy;
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
    /// Get a random viable policy based on the car state
    /// @param carState Car state inquired
    std::tuple<int, int> getRandomPolicy(state_tuple carState) const;
};

#endif /* TrackPolicy_hpp */

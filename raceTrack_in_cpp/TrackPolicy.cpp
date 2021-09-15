//
//  TrackPolicy.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/21/21.
//

#include "TrackPolicy.hpp"

TrackPolicy::TrackPolicy(double epsilonInput, int stateVisitThreshInput):
epsilonSoft(epsilonInput),
stateVisitThreshold(stateVisitThreshInput)
{
    std::cout << "Track Policy initialized" << "\n";
};


void TrackPolicy::updateStateActionVal(state_tuple carState, std::tuple<int, int> acc,
                                       double newReturn, double newWeight)
{
    // If never seen this state, initialize it
    if (stateActionSpace.find(carState) == stateActionSpace.end()) {
        // Get all available actions
        std::vector<std::tuple<int, int>> allAvailableAcc;
        allAvailableAcc = actionSpace(carState);
        
        // Add car state and all available actions into the state-aciton space map
        stateActionSpace.insert(std::pair<state_tuple, std::vector<std::tuple<int, int>>>
                                (carState, allAvailableAcc));
        
        // Add car state and weight into the state-action weight map
        std::vector<double> weightInit (static_cast<int>(allAvailableAcc.size()), 0.0);
        stateActionWeight.insert(std::pair<state_tuple, std::vector<double>>
                                 (carState, weightInit));
        
        // Add car state and value into the state-action value map
        std::vector<double> valueInit (static_cast<int>(allAvailableAcc.size()), 0.0);
        stateActionValue.insert(std::pair<state_tuple, std::vector<double>>
                                (carState, valueInit));
        
        // Initialize state visit count
        stateVisitCount.insert(std::pair<state_tuple, int> (carState, 0));
    }
    
    // Iterator for the action space
    std::vector<std::tuple<int, int>>::iterator iterAction;
    // Find the iterator to the action
    iterAction = std::find(stateActionSpace.at(carState).begin(),
                           stateActionSpace.at(carState).end(), acc);
        
    // Find the index of the inquired action
    int idxAction = static_cast<int>(std::distance(stateActionSpace.at(carState).begin(), iterAction));
    // Update state-aciton weight
    stateActionWeight.at(carState)[idxAction] += newWeight;
    // Store the current total weigth for convenience
    double currWeigth = stateActionWeight.at(carState)[idxAction];
    // Get the current state-action value for convenience
    double currVal = stateActionValue.at(carState)[idxAction];
    // Update state-aciton value
    stateActionValue.at(carState)[idxAction] += newWeight/currWeigth * (newReturn - currVal);
        
    // Update state visit count
    stateVisitCount.at(carState) += 1;
    
};


double TrackPolicy::getStateActionVal(state_tuple carState, std::tuple<int, int> acc) const
{
    // Iterator for the action space
    std::vector<std::tuple<int, int>>::const_iterator iterAction;
    // Find the iterator to the action
    iterAction = std::find(stateActionSpace.at(carState).begin(), stateActionSpace.at(carState).end(), acc);
    // Find the index of the inquired action
    int idxAction = static_cast<int>(std::distance(stateActionSpace.at(carState).begin(), iterAction));
    // Return state-action value according to the same index
    return stateActionValue.at(carState)[idxAction];
};


void TrackPolicy::setBehaveEpsilon(double epsilon)
{
    epsilonSoft = epsilon;
};


double TrackPolicy::getBehaveEpsilon() const
{
    return epsilonSoft;
};


std::tuple<int, int> TrackPolicy::getBehavePolicy(state_tuple carState) const
{
    // System clock Mersenne engine
    static std::mt19937 mersenneEng
    {static_cast<std::mt19937::result_type>(std::time(nullptr))};
    // Random 0 to 1 RNG
    static std::uniform_real_distribution<double> behaveRNG (0.0, 1.0);
    double behaveRandNum = behaveRNG(mersenneEng);
    // A dynamic epsilon:
    double epsilonDynamic;
    // If car state is new:
    // Can only choose a completely random action
    if (stateActionSpace.find(carState) == stateActionSpace.end()) {
        epsilonDynamic = 1.0;
    }
    // If car state has been visited:
    // equal to epsilonSoft if state visit count is less than a threshold
    // grow smaller as visit count increases further
    // This ensures behaviro policy maximizes exploration when state visit count is low
    // And converges to greedy policy when state visit count is high
    else {
        epsilonDynamic = epsilonSoft;
        double maxStateActionVal (*std::max_element(stateActionValue.at(carState).begin(),
                                                    stateActionValue.at(carState).end()));
//        epsilonDynamic = (maxStateActionVal >= 0) ?
//        1.0 : (epsilonSoft/exp(stateVisitCount.at(carState)/stateVisitThreshold));
        epsilonDynamic = (maxStateActionVal >= 0) ? 1.0 : epsilonSoft;
    }
    
    // With epsilonDynamic probability, choose a random viable action
    if (behaveRandNum <= epsilonDynamic) {
        return getRandomPolicy(carState);
    }
    // With 1-epsilonDynamic probability, choose the greedy action
    else {
        return getTargetPolicy(carState);
    }
};


double TrackPolicy::getBehaveProb(state_tuple carState, std::tuple<int, int> acc) const
{
    // Get all available actions
    std::vector<std::tuple<int, int>> allAvailableAcc;
    allAvailableAcc = actionSpace(carState);
    
    // Get the dynamic epsilon from the getBehavePolicy method
    double epsilonDynamic;
    if (stateActionSpace.find(carState) == stateActionSpace.end()) {
        epsilonDynamic = 1.0;
        return epsilonDynamic/static_cast<int>(allAvailableAcc.size());
    }
    else{
        epsilonDynamic = epsilonSoft;
        double maxStateActionVal (*std::max_element(stateActionValue.at(carState).begin(),
                                                    stateActionValue.at(carState).end()));
//        epsilonDynamic = (maxStateActionVal >= 0) ?
//        1.0 : (epsilonSoft/exp(stateVisitCount.at(carState)/stateVisitThreshold));
        epsilonDynamic = (maxStateActionVal >= 0) ? 1.0 : epsilonSoft;
        
        // If the action is the greedy action
        if (acc == getTargetPolicy(carState)) {
            return 1-epsilonDynamic + epsilonDynamic/static_cast<int>(allAvailableAcc.size());
        }
        // Or a random action
        else {
            return epsilonDynamic/static_cast<int>(allAvailableAcc.size());
        }
    }
};


std::tuple<int, int> TrackPolicy::getTargetPolicy(state_tuple carState) const
{
    // If state has not been seen before
    if (stateActionSpace.find(carState) == stateActionSpace.end()) {
        return getRandomPolicy(carState);
    }
    // If state has been seen
    else {
        // Find argmax of state-action value
        std::vector<double>::const_iterator iterVal;
        iterVal = std::max_element(stateActionValue.at(carState).begin(), stateActionValue.at(carState).end());
        int idxMaxVal = static_cast<int>(std::distance(stateActionValue.at(carState).begin(), iterVal));
        double maxStateActionVal = stateActionValue.at(carState)[idxMaxVal];
        // Return target (greedy) policy for the inquired state
        return (maxStateActionVal >= 0) ? getRandomPolicy(carState) : stateActionSpace.at(carState)[idxMaxVal];
    }
};


std::tuple<int, int> TrackPolicy::getRandomPolicy(state_tuple carState) const
{
    // System clock Mersenne engine
    static std::mt19937 mersenneEng
    {static_cast<std::mt19937::result_type>(std::time(nullptr))};
    // Get all available actions
    std::vector<std::tuple<int, int>> allAvailableAcc;
    allAvailableAcc = actionSpace(carState);
    // Choose and return a random one
    rand_uni_int accRNG (0, static_cast<int>(allAvailableAcc.size())-1);
    return allAvailableAcc[accRNG(mersenneEng)];
};


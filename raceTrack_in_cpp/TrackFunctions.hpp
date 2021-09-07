//
//  TrackFunctions.hpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/17/21.
//

#ifndef TrackFunctions_hpp
#define TrackFunctions_hpp

#include "TrackData.hpp"
#include "TrackEnv.hpp"
#include "TrackPolicy.hpp"
#include <vector>
#include <random>

using state_action_reward = std::tuple<state_tuple, std::tuple<int, int>, double>;
class TrackPolicy;

/// Returns available action (acceleration) space given the current car state
/// @param carState The current car state
std::vector<std::tuple<int, int>> actionSpace(state_tuple carState);

/// Generates a complete episode following behavior policy
/// @param inputTrackEnv Track environment input
/// @param inputTrackPolicy Track policy input
std::vector<state_action_reward> generateBehaveEpisode(const TrackEnv& inputTrackEnv, const TrackPolicy& inputTrackPolicy);

/// Generates a complete episode following target policy
/// @param inputTrackEnv Track enviroment input
/// @param inputTrackPolicy Track policy input
std::vector<state_action_reward> generateTargetEpisode(const TrackEnv& inputTrackEnv, const TrackPolicy& inputTrackPolicy);

#endif /* TrackFunctions_hpp */

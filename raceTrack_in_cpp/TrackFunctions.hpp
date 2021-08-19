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
#include <iostream>
#include <vector>
#include <random>

/// Returns available action (acceleration) space given the current car state
/// @param carState The current car state
std::vector<std::tuple<int, int>> actionSpace(state_tuple carState);



#endif /* TrackFunctions_hpp */

//
//  main.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/1/21.
//

#include "TrackData.hpp"
#include "TrackVisualizer.hpp"
#include "TrackEnv.hpp"
#include "TrackFunctions.hpp"
#include <iostream>

int main() {
    
    // Size of race track
    int trackSize (30);
    // Construct the race track
    TrackData raceTrack {trackSize};
    // Construct the environment
    TrackEnv raceEnv {raceTrack};
    // Construct the initial policies
    TrackPolicy racePolicy;
    // Construct the Visualizer
    TrackVisualizer raceVisualizer {raceTrack};
    // Initialize the episode vector, storing all state-actions in a single episode
    std::vector<state_action_reward> episode;
    // Record the number of episodes
    int episodeNum (0);
    // Record full episodes backtracked
    int fullEpisodeCount (0);
    
    // Discount parameter
    double gamma (1.0);
    
    // Start MC training
    std::cout << "Starting Monte-Carlo training..." << "\n";
    
    while (fullEpisodeCount < 1000) {
        // Adjust the behavior policy epsilon
        racePolicy.setBehaveEpsilon(1);
        // Generate a behavior episode
        episode = generateBehaveEpisode(raceEnv, racePolicy);
        ++ episodeNum;
        // Reset the discounted return
        double G (0);
        // Reset the importance sampling weight
        double W (1);
        // Declare a step of an episode
        state_action_reward step;
        // Looping through the episode
        while (!episode.empty()) {
            // Get the last step
            step = episode.back();
            episode.pop_back();
            // Update return
            G = gamma * G + std::get<2>(step);
            // Update state action value
            racePolicy.updateStateActionVal(std::get<0>(step), std::get<1>(step), G, W);
            // Only make further updates if behavior and target policy align
            if (std::get<1>(step) != racePolicy.getTargetPolicy(std::get<0>(step))) {
                break;
            }
            // Update weight of importance sampling
            W = W * 1/racePolicy.getBehaveProb(std::get<0>(step), std::get<1>(step));
        }
        // If the full episode is traversed
        if (episode.empty()) {
            ++ fullEpisodeCount;
            if (!fullEpisodeCount%10) {
                std::cout << fullEpisodeCount << "\n";
            }
        }
    }
    
    std::cout << "Off-policy Monte-Carlo control finished with " << episodeNum << " episodes." << "\n";
    
    int testNum (0);
    // Look at 100 samples of target policy
    while (testNum < 100) {
        // Generate episodes following target policy and visualize
        episode = generateTargetEpisode(raceEnv, racePolicy);
        std::cout << "Episode has " << episode.size() << " steps." << "\n";
        raceVisualizer.drawRaceTrack(episode);
        
        //Increment counter
        ++ testNum;
    }
    
    
    return 0;
}


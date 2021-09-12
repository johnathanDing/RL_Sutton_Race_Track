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
#include <thread>
#include <chrono>

int main() {
    
    // Size of race track
    int trackSize (30);
    // Construct the race track
    TrackData raceTrack (trackSize);
    // Construct the environment
    TrackEnv raceEnv (raceTrack);
    // Construct the initial policies
    TrackPolicy racePolicy {0.1, 100};
    // Construct the Visualizer
    TrackVisualizer raceVisualizer (raceTrack);
    // Initialize the episode vector, storing all state-actions in a single episode
    std::vector<state_action_reward_prob> episode;
    // Record the number of episodes
    int episodeNum (0);
    // Record how many steps learnt
    int stepsLearnt (0);
    int maxStepsLearnt (0);
    // Record full episodes backtracked
    int fullEpisodeCount (0);
    // Record the lowest index position learnt
    int minRow (INT_MAX);
    int minCol (INT_MAX);
    
    // Discount parameter
    double gamma (1.0);
    
    // Check the race track before starting MC training
    raceVisualizer.drawRaceTrack(std::vector<state_action_reward_prob> {{std::make_tuple(std::make_tuple(0,0,0,0), std::make_tuple(0,0),0, 0)}});
    
    // Start MC training
    std::cout << "Starting Monte-Carlo training..." << "\n";
    
    while (fullEpisodeCount < 100) {
        // Generate a behavior episode
        episode = generateBehaveEpisode(raceEnv, racePolicy);
        ++ episodeNum;
        stepsLearnt = 0;
        // Check episode size once in a while
        if (episodeNum%1000 == 1) {
            std::cout << "The " << episodeNum << " th episode's length is " << episode.size() << "\n";
        }
        // Reset the discounted return
        double G (0.0);
        // Reset the importance sampling weight
        double W (1.0);
        // Declare a step of an episode
        state_action_reward_prob step;
        // Looping through the episode
        while (!episode.empty()) {
            // Get the last step
            step = episode.back();
            episode.pop_back();
            ++ stepsLearnt;
            // Update return
            G = gamma * G + std::get<2>(step);
            // Update state action value
            racePolicy.updateStateActionVal(std::get<0>(step), std::get<1>(step), G, W);
            // Mark if start line is reached in back-tracking
            if (std::get<0>(std::get<0>(step)) == 0) {
                ++ fullEpisodeCount;
                std::cout << "Start line reached" << "\n";
            }
            // Only make further updates if behavior and target policy align
            if (std::get<1>(step) != racePolicy.getTargetPolicy(std::get<0>(step))) {
                minRow = std::min(minRow, std::get<0>(std::get<0>(step)));
                minCol = std::min(minCol, std::get<1>(std::get<0>(step)));
                maxStepsLearnt = std::max(maxStepsLearnt, stepsLearnt);
                if (episodeNum%1000 == 1) {
                    std::cout << minRow << ", " << minCol << ", " << maxStepsLearnt << "\n";
                }
                break;
            }
            // Update weight of importance sampling
            W = W * 1.0/std::get<3>(step);
        }
    }
    
    std::cout << "Off-policy Monte-Carlo control finished with " << episodeNum-1 << " episodes." << "\n";
    
    int testNum (0);
    // Look at 100 samples of target policy
    while (testNum < 10) {
        // Generate episodes following target policy and visualize
        episode = generateTargetEpisode(raceEnv, racePolicy);
        std::cout << "Episode has " << episode.size() << " steps." << "\n";
        raceVisualizer.drawRaceTrack(episode);
        
        //Increment counter
        ++ testNum;
    }
    
    
    return 0;
}


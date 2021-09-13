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
#include <fstream>
#include <thread>
#include <time.h>

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
    // Record the number of episodes of average episode length
    int episodeNum (0);
    int episodeLengthAve (0);
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
    // Open a file to record training data
    std::ofstream episodeFile ("Episode_File.txt");
    if (!episodeFile) {
        // If somehow the file cannot be opened
        std::cerr << "Episode recording file failed to open!" << "\n";
        return 1;
    }
    episodeFile << "# Per thousand episode average Return:" << "\n";
    episodeFile << "# No. of thousand episodes | Average Return" << "\n";
    // Start timer
    clock_t startTime (clock());
    
    while (fullEpisodeCount < 1000) {
        // Generate a behavior episode
        episode = generateBehaveEpisode(raceEnv, racePolicy);
        ++ episodeNum;
        episodeLengthAve += (static_cast<int>(episode.size())-episodeLengthAve)/((episodeNum-1)%1000+1);
        stepsLearnt = 0;
        // Check episode size once in a while
        if (episodeNum%1000 == 0) {
            std::cout << "The " << episodeNum/1000 << " th 1000 episode average length is " << episodeLengthAve << "\n";
            episodeFile << episodeNum/1000 << " " << -1*episodeLengthAve << "\n";
            episodeLengthAve = 0;
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
                if (episodeNum%1000 == 0) {
                    std::cout << minRow << ", " << minCol << ", " << maxStepsLearnt << "\n";
                }
                break;
            }
            // Update weight of importance sampling
            W = W * 1.0/std::get<3>(step);
        }
    }
    
    std::cout << "Off-policy Monte-Carlo control finished with " << episodeNum-1 << " episodes." << "\n";
    // Close recording file
    episodeFile.close();
    // Print total training time
    clock_t trainingTime (clock() - startTime);
    std::cout << "Total training time is: " << static_cast<double>(trainingTime)/CLOCKS_PER_SEC/3600 << " hrs." << "\n";
    
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


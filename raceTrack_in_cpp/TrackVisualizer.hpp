//
//  TrackVisualizer.hpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/13/21.
//

#ifndef TrackVisualizer_hpp
#define TrackVisualizer_hpp

#include "TrackData.hpp"
#include "TrackEnv.hpp"
#include "TrackFunctions.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

/// Class that creates a window to visualize race track and car movement
class TrackVisualizer
{
private:
    int trackSize;
    // A constant reference to given race track
    const vector_2D& raceTrack;
    // Container of a SFML window
    sf::RenderWindow trackWindow;
    // Pixel size of a single grid
    float gridPixel;
    
public:
    /// TrackVisualizer constructor
    /// @param inputTrack Input TrackData class instance.
    TrackVisualizer(const TrackData& inputTrack);
    
    /// Draws the race track given a car trajectory
    /// @param carEpisode A full trajectory of car episode
    void drawRaceTrack(std::vector<state_action_reward_prob> carEpisode);
    
private:
    /// Starts a race track window
    void startWindow();
    
    /// Draw race track grid to window
    void drawTrackGrid();
    
    /// Draws the current position of car
    /// @param carState tuple representing the current state of car
    void drawCarState(state_tuple carState);
};

#endif /* TrackVisualizer_hpp */

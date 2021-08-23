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
#include <iostream>
#include <SFML/Graphics.hpp>

using tuple_list = std::vector<std::tuple<int, int, int, int>>;

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
    /// @param carTrajectory A full trajectory (list) of car states
    void drawRaceTrack(tuple_list carTrajectory);
    
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

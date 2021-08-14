//
//  TrackVisualizer.hpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/13/21.
//

#ifndef TrackVisualizer_hpp
#define TrackVisualizer_hpp

#include "TrackData.hpp"
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
    // Constructor takes in race track info from TrackData
    TrackVisualizer(const TrackData& inputTrack);
    // Public method for drawing race track given car state array
    void drawRaceTrack(tuple_list carTrajectory);
    
private:
    // Open a window
    void startWindow();
    // Draw race track grid on an open window
    void drawTrackGrid();
    // Draw car position on grid
    void drawCarState(std::tuple<int, int, int, int> carState);
};

#endif /* TrackVisualizer_hpp */

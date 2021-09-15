//
//  TrackVisualizer.cpp
//  raceTrack_in_cpp
//
//  Created by Xiaoyue Ding on 8/13/21.
//

#include "TrackVisualizer.hpp"


TrackVisualizer::TrackVisualizer(const TrackData& inputTrack):
trackSize (inputTrack.getTrackSize()),
raceTrack (inputTrack.getRaceTrack()),
gridPixel (20.0)
{
    std::cout << "Track Visualizer constructed..." << "\n";
};


void TrackVisualizer::startWindow()
{
    int windowHeight {trackSize * static_cast<int>(gridPixel)};
    int windowWidth  {trackSize * static_cast<int>(gridPixel)};
    trackWindow.create(sf::VideoMode(windowWidth, windowHeight), "Xiaoyue's Race Track");
    trackWindow.setVerticalSyncEnabled(true);
};


void TrackVisualizer::drawTrackGrid()
{
    // Set up a single grid with inward outlines
    sf::RectangleShape singleGrid (sf::Vector2f(gridPixel, gridPixel));
    singleGrid.setOutlineColor(sf::Color::Black);
    singleGrid.setOutlineThickness(-1);
    
    for (int i_row{0}; i_row < trackSize; ++i_row) {
        for (int i_col{0}; i_col < trackSize; ++i_col) {
            // Only need to plot if there is race track here
            if (raceTrack[i_row][i_col] == 1) {
                // Want the starting line to be at bottom
                singleGrid.setPosition(i_col*gridPixel, (trackSize-1-i_row)*gridPixel);
                // If starting line, make it green
                if (i_row == 0) {
                    singleGrid.setFillColor(sf::Color::Green);
                }
                // If finish line, make it blue
                else if (i_col == trackSize-1){
                    singleGrid.setFillColor(sf::Color::Blue);
                }
                // If mid way, make it red carpet
                else {
                    singleGrid.setFillColor(sf::Color::Red);
                }
                trackWindow.draw(singleGrid);
            }
        }
    }
};


void TrackVisualizer::drawCarState(state_tuple carState)
{
    // Set up car squre object
    sf::RectangleShape carGrid (sf::Vector2f(gridPixel, gridPixel));
    carGrid.setFillColor(sf::Color::White);
    // Car position
    float car_x = std::get<1>(carState)*gridPixel;
    float car_y = (trackSize-1-std::get<0>(carState))*gridPixel;
    carGrid.setPosition(car_x, car_y);
    trackWindow.draw(carGrid);
};


void TrackVisualizer::drawRaceTrack(std::vector<state_action_reward_prob> carEpisode)
{
    // Get the length of the car trajectory
    int trajLength {static_cast<int>(carEpisode.size())};
    // Starting index
    int i_pos {0};
    // Open the window if it's not already
    if (trackWindow.isOpen() == false)
    {
        startWindow();
    };
    // Grid-drawing loop
    while (trackWindow.isOpen())
    {
        sf::Event event;
        while (trackWindow.pollEvent(event))
        {
            // If close, close
            if (event.type == sf::Event::Closed)
            {
                std::cout << "User closed the window." << "\n";
                trackWindow.close();
            }
            // If key is pressed and it's space, move to next car state
            if ((event.type == sf::Event::KeyPressed)
                && (event.key.code == sf::Keyboard::Space))
            {
                ++i_pos;
            }
            // If trajectory is depleted, also close
            if (i_pos == trajLength)
            {
                std::cout << "Trajectory finished." << "\n";
                trackWindow.close();
            }
            
            // Clear the window for the next update
            trackWindow.clear(sf::Color::Black);
            // Draw the track grid first
            drawTrackGrid();
            // Then draw the current car state
            drawCarState(std::get<0>(carEpisode[i_pos]));
            
            // Push the draw buffer to display
            trackWindow.display();
        }
    }
};

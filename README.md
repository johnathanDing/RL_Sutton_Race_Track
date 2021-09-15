# Sutton's Autonomous Racetrack with Off-policy Monte-Carlo control

![Target Episode Demo](./Examples/OneEpisodeRecording_2.gif)

This is a simple attempt to solve the race track problem in Sutton's book, using Off-policy Monte-Carlo control method.

This solution was written in C++ as main language, on Xcode 12.X platform. Visualization of race track requires installation of the SFML packages.
Unit testings were carried out using XCTest as well. All modules have tests except the visualization module, which cannot be supported by XCTest.

## Problem Statement:
Consider driving a race car in racetracks like those shown in the following figure from the book. You want to go as fast as possible, but not so fast as to run off the track. In our simplified racetrack, the car is at one of a discrete set of grid positions, the cells in the diagram. The velocity is also discrete, a number of grid cells moved horizontally and vertically per time step.

![raceTrack problem statement](./Examples/Problem_Statement.png)

The actions are increments to the velocity components. Each may be changed by +1, −1, or 0 in one step, for a total of nine actions. Both velocity components are restricted to be nonnegative and less than 5, and they cannot both be zero except at the starting line. Each episode begins in one of the randomly selected start states with both velocity components zero and ends when the car crosses the finish line. The reward are -1 for each step until the car crosses the finish line. If the car hits the track boundary, it is moved back to a random position on the starting line, both velocity components are reduced to zero, and the episode continues. Before updating the car's location at each time step, check to see if the projected path of the car intersects the track boundary. If it intersects the finish line, the episode ends; if it intersects anywhere else, the car is considered to have hit the track boundary and is sent back to the starting line.

Apply a Monte Carlo control method to this task to compute the optimal policy from each starting state.

## Solution Overview:
The problem mimics a over-simplified model of an autonomous race car driving on race track. Since there's no straightforward analytical model of the entire race track environment (the number of combinations of states can be astronimical), the use of Monte Carlo control seems a good option.

We choose to use Off-Policy Monte Carlo control for this problem. The reason we do so is that we prefer a deterministic (greedy) target policy after training (General Policy Iterations), since we're driving a race car and wouldn't want much randomness in our policy. Off-Policy control enables us to do that through using a different, behaivor policy during the GPI training process. 

The advantage of Off-Policy control is that behavior policy, which could even change in between or during episodes, can continue exploring action options while target policy converges to an optimal policy. The disadvantage is that the training can be slow compared to on-policy control, due to the nature of behavior policy exploration and importance sampling required. 

We'll encounter both of these advantage and disadvantage during our solution process.

## Essential Modules
We start tackling our problem by first preparing all the modules essential to our Monte Carlo reinforcement learning problem: the environment (race track and response), the state-action space and policies, Monte Carlo episode generators, and the visualizer of the race track.

### The race track environment
We study square race tracks without loosing generality. A race track has its starting line on the bottom of the square board, gradually steers towards right with some random walk. Once past a certain point (row), the track has a high chance of going straight right, and ends on the finish line on the right boarder of the square board.

Sample visualizations of some viable race tracks can be as following:
![raceTrack example](./Examples/RaceTrack_Examples.png)

The race track is generated through a dedicated class, TrackData:
```cpp
using vector_2D = std::vector<std::vector<int>>;

/// Class that generates and stores race track
class TrackData
{
private:
    // Edge length of square-shaped race track
    const int trackSize;
    // 2D vector of race track
    vector_2D raceTrack;
public:
    /// TrackData class constructor
    /// @param trackSizeInput How many squares do you want the race track edge to be? Default is 30.
    TrackData(int trackSizeInput = 30);
    /// Returns track edge size
    int getTrackSize() const;
    /// Returns race track as a 2D vector constant reference
    const vector_2D& getRaceTrack() const;
private:
    /// Generate the race track according to author-defined rules
    void generateRaceTrack();
};
```

We then construct an environment that transitions a given state-action pair to the next state, while also giving feedback on the reward and whether the finish line is crossed. As per the problem statement, if car races out of track, it will be reset to a random starting line position unless the finishe line is crossed.

```cpp
using state_tuple = std::tuple<int, int, int, int>;
/// Struct storing env response including reward, next state, and whether the finish line has been crossed
struct envResponse
{
    // Reward to the step
    int reward;
    // The next state(four-) tuple
    state_tuple nextState;
    // Whether the finish line is crossed at this step
    bool finished;
};
/// Class that simulates the track environment
class TrackEnv
{
private:
    int trackSize;
    // A constant reference to given race track
    const vector_2D& raceTrack;
    // Easy access to starting line positions
    std::vector<std::tuple<int, int>> startLine;
    // Easy access to finish line positions
    std::vector<std::tuple<int, int>> finishLine;
public:
    /// Construct track parameters based on input TrackData
    /// @param inputTrack Input TrackData class instance
    TrackEnv(const TrackData& inputTrack);
    /// Returns a random starting state as state (4-) tuple
    state_tuple getStartState() const;
    /// Given current state and action (acceleration), return the next environment response struct
    /// @param currState Current state tuple
    /// @param acceleration Chosen action
    envResponse getEnvResponse(state_tuple currState, std::tuple<int, int> acceleration) const;
};
```

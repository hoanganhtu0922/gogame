#pragma once
#include <chrono>

class Timer {
public:
    using clock = std::chrono::steady_clock;
    std::chrono::time_point<clock> start_time;
    bool running = false;
    long long paused_time = 0; 

    void start();

    void pause();

    void resume();

    long long elapsedMicro();

    int elapsedSeconds();
};

#include "timer.h"

void Timer::start() {
    running = 0;
}

void Timer::pause() {
    if (running) {
        paused_time += std::chrono::duration_cast<std::chrono::microseconds>(
            clock::now() - start_time
        ).count();
        running = false;
    }
}

void Timer::resume() {
    if (!running) {
        start_time = clock::now();
        running = true;
    }
}

long long Timer::elapsedMicro() {
    if (running) {
        return paused_time +
               std::chrono::duration_cast<std::chrono::microseconds>(
                   clock::now() - start_time
               ).count();
    }
    return paused_time;
}

int Timer::elapsedSeconds() {
    return elapsedMicro() / 1000000;
}

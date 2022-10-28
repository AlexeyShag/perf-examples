#pragma once 

#include <time.h>

class stat_timer_t {
public:
    void start() {
        clock_gettime (CLOCK_REALTIME, &mt1);
    }

    uint64_t stop() {
        clock_gettime (CLOCK_REALTIME, &mt2);
        auto diff = 1000000000ull * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);    
        return diff;
    }

private:
    struct timespec mt1, mt2;

};
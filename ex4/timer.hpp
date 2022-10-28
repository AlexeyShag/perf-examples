#pragma once 

#include <time.h>

struct data_t
{
    uint64_t value{0};
};


class stat_timer_t {
public:
    void start() {
        clock_gettime (CLOCK_REALTIME, &mt1);
    }

    void stop() {
        clock_gettime (CLOCK_REALTIME, &mt2);
    }

    void reset() {

    }

    data_t get_data() {
        data_t data;
        data.value = 1000000000ull * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);    
        return data;
    } 

private:
    struct timespec mt1, mt2;

};
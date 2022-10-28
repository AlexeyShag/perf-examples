#include "measurer.hpp"
#include "timer.hpp"
#include <vector>
#include <deque>
#include <list>
#include <iostream>

static constexpr const size_t __page_size = 4096;

template<typename base_measurer_t>
void page_fault_test(base_measurer_t& measurer) { 
    static constexpr const size_t __size = __page_size * 100;
    std::vector<int> a;
    a.reserve(__size);
    
    // first iteration
    measurer.start();
    for (size_t i = 0; i < __size; ++i) {
        a.push_back(i);
    }
    measurer.stop();

    auto data = measurer.get_data();
    std::cout << "First iteration. Statitic: " << data.value << '\n';

    // clean up vector
    a.clear();

    // reset measurer counter
    measurer.reset();

    // second iteration
    measurer.start();
    for (size_t i = 0; i < __size; ++i) {
        a.push_back(i);
    }
    measurer.stop();

    data = measurer.get_data();
    std::cout << "Second iteration. Statitic: " << data.value << '\n';
}

template<typename base_measurer_t>
void data_cache_test(base_measurer_t& measurer) {
    static constexpr const size_t __size = __page_size * 100;
    std::vector<int> a;
    a.resize(__size);

    std::cerr << "======[vector]=====\n";
    for (int j = 0; j < 5; ++j) {
        measurer.reset();
        measurer.start();
        for (size_t i = 0; i < __size; ++i) {
            a[i] = i;
        }
        measurer.stop();

        auto data = measurer.get_data();
        std::cout << "vector: " << j << " iteration. Statistic: " << data.value << '\n';
    }

    std::deque<int> d;
    d.resize(__size);

    std::cerr << "======[deque]=====\n";
    for (int j = 0; j < 5; ++j) {
        measurer.reset();
        measurer.start();
        for (size_t i = 0; i < __size; ++i) {
            d[i] = i;
        }
        measurer.stop();

        auto data = measurer.get_data();
        std::cout << "deque:  " << j << " iteration. Statistic: " << data.value << '\n';
    }

    std::list<int> l;
    for (int i = 0; i < __size; ++i) {
        l.push_back(i);
    }

    std::cerr << "======[list]=====\n";
    for (int j = 0; j < 5; ++j) {
        measurer.reset();
        measurer.start();
        int i = 0;
        for (auto& elem: l) {
            elem = ++i;
        }
        measurer.stop();

        auto data = measurer.get_data();
        std::cout << "list:   " << j << " iteration. Statistic: " << data.value << '\n';
    }
}

signed main() {

    {
        // stat_timer_t timer;
        // page_fault_test(timer);

        // measurer_t measurer(create_page_fault_attr());
        // page_fault_test(measurer);
    }

    {
        // std::cout << "============= timer ===============\n";
        // stat_timer_t timer;
        // data_cache_test(timer);

        std::cout << "============= perf ===============\n";
        measurer_t measurer(create_data_cache_attr());
        data_cache_test(measurer);
    }
    
    
    


    return 0;
}
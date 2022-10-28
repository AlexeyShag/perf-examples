#pragma once 

#include <linux/perf_event.h>    /* Definition of PERF_* constants */
#include <linux/hw_breakpoint.h> /* Definition of HW_* constants */
#include <sys/syscall.h>         /* Definition of SYS_* constants */
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <cstdio>
#include <errno.h>

#include <stdexcept>
#include <cstring>


perf_event_attr create_page_fault_attr() {
    perf_event_attr attributes;
    memset(&attributes, 0, sizeof(perf_event_attr));

    attributes.disabled = 1;
    attributes.size = sizeof(attributes);

    attributes.type = PERF_TYPE_SOFTWARE;
    attributes.config = PERF_COUNT_SW_PAGE_FAULTS;

    return attributes;
}

perf_event_attr create_data_cache_attr() {
    perf_event_attr attributes;
    memset(&attributes, 0, sizeof(perf_event_attr));

    attributes.disabled = 1;
    attributes.size = sizeof(attributes);

    attributes.type = PERF_TYPE_HW_CACHE;
    attributes.config = PERF_COUNT_HW_CACHE_L1D |
                        (PERF_COUNT_HW_CACHE_OP_READ << 8) |
                        (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

    return attributes;
}

struct read_data_t {
    uint64_t value{0};
    uint64_t time_enabled{0};
    uint64_t time_running{0};
    uint64_t id{0};
};

class measurer_t {
public:
    measurer_t(perf_event_attr attributes) : attributes_(attributes) {
        perf_descriptor_ = syscall(
            SYS_perf_event_open, 
            &attributes_,
            getpid(),
            -1,
            -1,
            PERF_FLAG_FD_NO_GROUP);

        if (perf_descriptor_ <= 0) {
            printf("Code: %d %s \n", errno, strerror(errno));
            throw std::runtime_error("error in perf");
        }
    }

    void start() {
        if (ioctl(perf_descriptor_, PERF_EVENT_IOC_ENABLE, 0) != 0) {
            printf("%s\n", strerror(errno));
            throw std::runtime_error("error in Enable");
        }
    }

    void stop() {
        if (ioctl(perf_descriptor_, PERF_EVENT_IOC_DISABLE, 0) != 0) {
            printf("%d %s\n", errno, strerror(errno));
            throw std::runtime_error("error in Disable");
        }
    }

    void reset() {
        if (ioctl(perf_descriptor_, PERF_EVENT_IOC_RESET, 0) != 0) {
            printf("%d %s\n", errno, strerror(errno));
            throw std::runtime_error("error in Reset");
        }
    }

    read_data_t get_data() {
        read_data_t data;
        memset(&data, 0, sizeof(data));
        if (read(perf_descriptor_, &data, sizeof(data)) < 0) {
            printf("Code: %d %s\n", errno, strerror(errno));
            throw std::runtime_error("error in GetData");
        }
        return data;
    }


private:
    perf_event_attr attributes_;
    int perf_descriptor_{-1};
};
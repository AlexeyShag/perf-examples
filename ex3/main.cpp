#include <iostream>

template<size_t level = 0>
uint64_t fibonacci(uint64_t i) {
    if constexpr (level >= 100) {
        return 1;
    } else {
        if (i <= 1) {
            return 1;
        }
        return fibonacci<level + 1>(i - 1) + fibonacci<level + 2>(i - 2);
    }
}

uint64_t biffer[100000];
uint64_t fast_fibonacci(uint64_t i) {
    if (biffer[i] != 0) {
        return biffer[i];
    }
    if (i <= 1) {
        return biffer[i] = 1;
    }
    return biffer[i] = fast_fibonacci(i - 1) + fast_fibonacci(i - 2);
}

int main() {
    uint64_t n = 0;
    std::cin >> n;
    std::cout << fibonacci(n);
    return 0;
}
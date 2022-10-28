#include <iostream>
#include <random>

int main() {

    std::mt19937 rnd(time(0));
    uint64_t n = 1000'000;
    uint64_t t = 1000'000;

    std::cout << n << ' ' << t << '\n';
    for (size_t i = 0; i < n; ++i) {
        int q = rnd() % 2;
        if (q == 0) {
            std::cout << "+" << ' ' << rnd() % n + 1 << ' ' << rnd() << '\n';
        } else {
            auto l = rnd() % n + 1;
            auto r = rnd() % n + 1;
            if (r < l) {
                std::swap(l, r);
            }
            std::cout << "?" << ' ' << l << ' ' << r << '\n';
        }
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include "timer.hpp"

using matrix_t = std::vector<std::vector<int64_t>>;

matrix_t slow_multiplication(matrix_t& a, matrix_t& b) {
    matrix_t c(a.size(), std::vector<int64_t>(b[0].size(), 0));
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b[0].size(); ++j) {
            for (size_t k = 0; k < b.size(); ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return std::move(c);
}

void T(matrix_t& matrix) {
    assert(matrix.size() == matrix[0].size());
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = i + 1; j < matrix[i].size(); ++j) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }
}

matrix_t fast_multiplication(matrix_t& a, matrix_t& b) {
    matrix_t c(a.size(), std::vector<int64_t>(b[0].size(), 0));
    T(b);
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b[0].size(); ++j) {
            for (size_t k = 0; k < b.size(); ++k) {
                c[i][j] += a[i][k] * b[j][k];
            }
        }
    }
    return std::move(c);
}

static std::mt19937_64 rnd(time(0));
static constexpr const size_t __mode = 1'000'000'007;

matrix_t get_random_matrix(size_t n, size_t m) {
    matrix_t matrix(n, std::vector<int64_t>(m));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            matrix[i][j] = rnd() % __mode;
        }
    }
    return std::move(matrix);
}

int main() {
    size_t n;
    std::cin >> n;

    matrix_t a = get_random_matrix(n, n);
    matrix_t b = get_random_matrix(n, n);
    stat_timer_t timer;

    /// slow
    timer.start();
    matrix_t c1 = slow_multiplication(a, b);
    auto ts = timer.stop();
    std::cerr << "Slow multiplication. Time: " << ts / 1'000'000'000 << " seconds, " << ts % 1'000'000'000 << " nanoseconds."<< '\n';

    /// fast
    timer.start();
    matrix_t c2 = fast_multiplication(a, b);
    ts = timer.stop();
    std::cerr << "Fast multiplication. Time: " << ts / 1'000'000'000 << " seconds, " << ts % 1'000'000'000 << " nanoseconds."<< '\n';

    assert(c1 == c2);
    return 0;
}
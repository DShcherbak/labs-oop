#pragma once

#include <random>
#include <iterator>
#include <ctime>

namespace utils {
/**
 * @brief provides const params for benchmarking
 */
struct BMParams {
    struct small {
        static constexpr std::size_t start_point = 1;
        static constexpr std::size_t end_point = 32;
        static constexpr std::size_t multiplier = 2;
    };

    struct big {
        static constexpr std::size_t start_point = 64;
        static constexpr std::size_t end_point = 16384; //131072
        static constexpr std::size_t multiplier = 2; //4
    };

    static constexpr int int_lower_bound = -10000;
    static constexpr int int_upper_bound = 10000;
};

inline int randomInt(const int &lower_bound, const int &upper_bound) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
    return dist(gen);
}

}

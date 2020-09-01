#pragma once

#include <random>
#include <iostream>
#include <iterator>
#include <ctime>


namespace utils {
/**
 * @brief provides const params for testing
 */
struct TestParam {
    static constexpr std::size_t size_small = 25;
    static constexpr std::size_t size_megium = 50;
    static constexpr std::size_t size_large = 100;
    static constexpr std::size_t size_extreme = 1000;
    static constexpr int int_lower_bound = -10000;
    static constexpr int int_upper_bound = 10000;

    static std::seed_seq& seed() {
        static std::random_device rd;
        static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};

        return seed;
    }
};

inline bool printSeedSeq(std::seed_seq &seed) {
    std::cout << "SEED SEQUENCE: ";
    seed.param(std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return {};
}

}

#include "catch.hpp"
#include "TestParam.hpp"

#include "../src/RedBlackTree.hpp"

#include <random>
#include <iostream>
#include <unordered_set>



TEST_CASE("Red-Black Tree test", "[Red-Black Tree]") {
    auto& seed = utils::TestParam::seed();

    auto randomInt = [&seed](int lower_bound, int upper_bound) {
        static std::mt19937_64 gen(seed);
        std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
        return dist(gen);
    };

    SECTION("int") {
        SECTION("Insert") {
            auto test = [randomInt = randomInt](RedBlackTreeTestable<int> &&tree, std::size_t test_size) {
                for (std::size_t i = 0; i < test_size; i++) {
                    auto key = randomInt(utils::TestParam::int_lower_bound, utils::TestParam::int_upper_bound);
                    tree.insert(key);
                    REQUIRE(tree.checkValues());
                    REQUIRE(tree.checkColors());
                    REQUIRE(tree.checkBlackHeights());
                }
            };

            test({}, utils::TestParam::size_small);
            test({}, utils::TestParam::size_megium);
            test({}, utils::TestParam::size_large);
            test({}, utils::TestParam::size_extreme);
        }

        SECTION("Remove") {
            auto test = [randomInt = randomInt, &seed](RedBlackTreeTestable<int> &&tree, std::size_t test_size) {
                std::vector keys(test_size, 0);
                for (auto &item : keys) {
                    item = randomInt(utils::TestParam::int_lower_bound, utils::TestParam::int_upper_bound);
                    tree.insert(item);
                }

                for (std::size_t i = 0; i < test_size / 2; i++) {
                    keys.push_back(randomInt(utils::TestParam::int_lower_bound, utils::TestParam::int_upper_bound));
                }

                std::shuffle(keys.begin(), keys.end(), std::mt19937_64(seed));
                for (auto &item : keys) {
                    tree.remove(item);
                    REQUIRE(tree.checkValues());
                    REQUIRE(tree.checkColors());
                    REQUIRE(tree.checkBlackHeights());
                }
            };

            test({}, utils::TestParam::size_small);
            test({}, utils::TestParam::size_megium);
            test({}, utils::TestParam::size_large);
            test({}, utils::TestParam::size_extreme);
        }

        SECTION("Include") {
            auto test = [randomInt = randomInt, &seed](RedBlackTreeTestable<int> &&tree, std::size_t test_size) {
                std::unordered_set<int> keys_set;
                for (std::size_t i = 0; i < test_size; i++) {
                    keys_set.insert(randomInt(utils::TestParam::int_lower_bound, utils::TestParam::int_upper_bound));
                }
                std::vector keys(keys_set.begin(), keys_set.end());
                for (auto &item : keys) {
                    REQUIRE(!tree.includes(item));
                    tree.insert(item);
                    REQUIRE(tree.includes(item));
                }

                std::shuffle(keys.begin(), keys.end(), std::mt19937_64(seed));
                for (auto &item : keys) {
                    tree.remove(item);
                    REQUIRE(!tree.includes(item));
                }
            };

            test({}, utils::TestParam::size_small);
            test({}, utils::TestParam::size_megium);
            test({}, utils::TestParam::size_large);
            test({}, utils::TestParam::size_extreme);
        }
    }
}

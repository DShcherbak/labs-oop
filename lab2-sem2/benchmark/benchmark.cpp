#include <benchmark/benchmark.h>

#include "BenchmarkUtils.hpp"

#include "../src/RedBlackTree.hpp"

#include "../src/Tree.hpp"
#include "../src/TreeAdapters.hpp"
#include "../src/TreeFactory.hpp"
#include "../src/TreeCommands.hpp"

enum class TreeType {
    RedBlack, B
};

static void BM_vector_iteration(benchmark::State &state) {
    std::vector keys(state.range(0), 0);

    for (auto _ : state) {
        for (const auto &item : keys) {}
    }
}

static void BM_insert(benchmark::State &state, const TreeType tree_type) {
    std::shared_ptr<Tree<int>> tree;

    switch (tree_type) {
        case TreeType::RedBlack:
            tree = TreeFactory::NewRedBlack<int>();
            break;

        case TreeType::B:
            tree = TreeFactory::NewB<int>();
            break;
    }

    std::vector keys(state.range(0), 0);
    for (auto &item : keys) {
        item = utils::randomInt(utils::BMParams::int_lower_bound, utils::BMParams::int_upper_bound);
    }

    for (auto _ : state) {
        for (auto &item : keys) {
            tree->insert(item);
        }
    }
}


static void BM_remove(benchmark::State &state, const TreeType tree_type) {
    std::shared_ptr<Tree<int>> tree;

    switch (tree_type) {
        case TreeType::RedBlack:
            tree = TreeFactory::NewRedBlack<int>();
            break;
        case TreeType::B:
            tree = TreeFactory::NewB<int>();
            break;
    }

    std::vector keys(state.range(0), 0);
    for (auto &item : keys) {
        item = utils::randomInt(utils::BMParams::int_lower_bound, utils::BMParams::int_upper_bound);
        tree->insert(item);
    }

    std::random_device rd;
    std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    std::shuffle(keys.begin(), keys.end(), std::mt19937_64(seed));

    for (auto _ : state) {
        for (auto &item : keys) {
            tree->remove(item);
        }
    }
}

static void BM_search(benchmark::State &state, const TreeType tree_type) {
    std::shared_ptr<Tree<int>> tree;

    switch (tree_type) {
        case TreeType::RedBlack:
            tree = TreeFactory::NewRedBlack<int>();
            break;
        case TreeType::B:
            tree = TreeFactory::NewB<int>();
            break;
    }

    std::vector keys(state.range(0), 0);
    for (auto &item : keys) {
        item = utils::randomInt(utils::BMParams::int_lower_bound, utils::BMParams::int_upper_bound);
        tree->insert(item);
    }

    std::random_device rd;
    std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    std::shuffle(keys.begin(), keys.end(), std::mt19937_64(seed));

    for (auto _ : state) {
        for (auto &item : keys) {
            tree->contains(item);
        }
    }
}

//VECTOR ITERATION
BENCHMARK(BM_vector_iteration)
        ->RangeMultiplier(utils::BMParams::small::multiplier)
        ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
        ->Unit(benchmark::kNanosecond);

BENCHMARK(BM_vector_iteration)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);

//RED-BLACK TREE INSERTION
BENCHMARK_CAPTURE(BM_insert, "red-black tree", TreeType::RedBlack)
        ->RangeMultiplier(utils::BMParams::small::multiplier)
        ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
        ->Unit(benchmark::kNanosecond);

BENCHMARK_CAPTURE(BM_insert, "red-black tree", TreeType::RedBlack)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);

//B-TREE INSERTION
BENCHMARK_CAPTURE(BM_insert, "b-tree", TreeType::B)
        ->RangeMultiplier(utils::BMParams::small::multiplier)
        ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
        ->Unit(benchmark::kNanosecond);

BENCHMARK_CAPTURE(BM_insert, "b-tree", TreeType::B)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);

//RED-BLACK TREE REMOVAL
BENCHMARK_CAPTURE(BM_remove, "red-black tree", TreeType::RedBlack)
        ->RangeMultiplier(utils::BMParams::small::multiplier)
        ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
        ->Unit(benchmark::kNanosecond);

BENCHMARK_CAPTURE(BM_remove, "red-black tree", TreeType::RedBlack)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);

//B-TREE REMOVAL
BENCHMARK_CAPTURE(BM_remove, "b-tree", TreeType::B)
        ->RangeMultiplier(utils::BMParams::small::multiplier)
        ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
        ->Unit(benchmark::kNanosecond);

BENCHMARK_CAPTURE(BM_remove, "b-tree", TreeType::B)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);

//RED-BLACK TREE SEARCH
BENCHMARK_CAPTURE(BM_search, "red-black tree", TreeType::RedBlack)
        ->RangeMultiplier(utils::BMParams::small::multiplier)
        ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
        ->Unit(benchmark::kNanosecond);

BENCHMARK_CAPTURE(BM_search, "red-black tree", TreeType::RedBlack)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);

//B-TREE SEARCH
BENCHMARK_CAPTURE(BM_search, "b-tree", TreeType::B)
        ->RangeMultiplier(utils::BMParams::small::multiplier)
        ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
        ->Unit(benchmark::kNanosecond);

BENCHMARK_CAPTURE(BM_search, "b-tree", TreeType::B)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);




BENCHMARK_MAIN();
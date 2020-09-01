#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "TestParam.hpp"


int main( int argc, char* argv[] ) {
    utils::printSeedSeq(utils::TestParam::seed());

    int result = Catch::Session().run( argc, argv );

    return result;
}

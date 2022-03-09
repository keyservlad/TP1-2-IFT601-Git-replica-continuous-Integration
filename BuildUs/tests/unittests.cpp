#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <buildus.h>

TEST_CASE("checkBuildusAndClean")
{
    boost::system::error_code code;

    const auto currentPath = boost::filesystem::current_path();
    const auto intermediatePath = currentPath / "intermediate";
    const auto mainPatho = intermediatePath / "main.o";
    const auto helloPatho = intermediatePath / "helloworld.o";
    const auto helloPathApp = currentPath / "tests" / "testsUtils" / "helloworld";

    // we execute Buildus and check that everything worked just fine
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/config.buildus tests/testsUtils") == 0);

    // we check that teh files and directory were created
    REQUIRE(boost::filesystem::exists(mainPatho, code));
    REQUIRE(boost::filesystem::exists(helloPatho, code));
    REQUIRE(boost::filesystem::exists(helloPathApp, code));
 
    // Execute the created hello world app
    REQUIRE(boost::process::system("./tests/testsUtils/helloworld") == 0);

    // execute clean and check that intermediate dir is gone
    REQUIRE(boost::process::system("./BuildUs clean") == 0);
    REQUIRE(!boost::filesystem::exists(intermediatePath, code));
}

TEST_CASE("cleanWithNoIntermediateDirectory")
{
    boost::system::error_code code;
    const auto intermediatePath = boost::filesystem::current_path() / "intermediate";

    // First clean to make sure there is no intermediate folder
    clean();
    REQUIRE(!boost::filesystem::exists(intermediatePath, code));

    // Clean, should fail
    REQUIRE(clean() == 1);
}

// now we can check that the tests fail in case of bad config (we made few cases)
TEST_CASE("inexistentConfig")
{
    // invalid config path or inexistant
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/inexistent.config tests/testsUtils") == 1);
}

TEST_CASE("invalidConfig")
{
    // config file does not end with .buildus
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/config.invalid tests/testsUtils") == 1);
}

TEST_CASE("invalidProjectname")
{
    // no project name in the config file
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/badconfig1.buildus tests/testsUtils") == 1);
}

TEST_CASE("compileMissingFile")
{
    // trying to compile a missing file
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/badconfig2.buildus tests/testsUtils") == 1);
}

TEST_CASE("noCompile")
{
    // no compile
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/badconfig3.buildus tests/testsUtils") == 1);
}


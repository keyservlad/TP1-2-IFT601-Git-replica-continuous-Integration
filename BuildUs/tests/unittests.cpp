#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <buildus.h>

TEST_CASE("compile_build_and_clean")
{
    boost::system::error_code code;

    // Get paths
    const auto originalPath = boost::filesystem::current_path();
    auto path = originalPath;
    const auto intermediatePath = path.append("intermediate");
    path = intermediatePath;
    const auto mainPath_o = path.append("main.o");
    path = intermediatePath;
    auto helloPatho = path.append("helloworld.o");
    path = originalPath;
    const auto helloPathApp = path.append("tests").append("testsUtils").append("helloworld");

    // Execute BuildUs
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/config.buildus tests/testsUtils") == 0);

    // Check if directory and file creation was a success
    REQUIRE(boost::filesystem::exists(mainPath_o, code));
    REQUIRE(boost::filesystem::exists(helloPatho, code));
    REQUIRE(boost::filesystem::exists(helloPathApp, code));
 
    // Execute the created app
    REQUIRE(boost::process::system("./tests/testsUtils/helloworld") == 0);

    // Clean intermediate directory
    REQUIRE(boost::process::system("./BuildUs clean") == 0);
    REQUIRE(!boost::filesystem::exists(intermediatePath, code));
}

TEST_CASE("config_inexistent_config")
{
    // Execute BuildUs, should fail
    REQUIRE(boost::process::system("./BuildUs invalid/folder/inexistent.config tests/testsUtils") == 1);
}

TEST_CASE("config_invalid_config")
{
    // Execute BuildUs, should fail (config file does not end with .buildus)
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/config.invalid tests/testsUtils") == 1);
}

TEST_CASE("config_invalid_projectname")
{
    // Execute BuildUs, should fail (no project name in the config file)
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/badconfig1.buildus tests/testsUtils") == 1);
}

TEST_CASE("compile_missing_file")
{
    // Execute BuildUs, should fail (trying to compile a missing file)
    REQUIRE(boost::process::system("./BuildUs tests/testsUtils/badconfig2.buildus tests/testsUtils") == 1);
}

TEST_CASE("clean_no_intermediate_directory")
{
    boost::system::error_code code;

    // Get paths
    const auto originalPath = boost::filesystem::current_path();
    auto path = originalPath;
    const auto intermediatePath = path.append("intermediate");

    // First clean to make sure there is no intermediate folder
    clean();
    REQUIRE(!boost::filesystem::exists(intermediatePath, code));

    // Clean, should fail
    REQUIRE(clean() == 1);
}
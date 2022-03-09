#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>

int build(std::string filePathStr, std::string destinationPathStr);
int clean();
std::string compile(YAML::Node config, std::string filePathStr);
std::string getLibrary(YAML::Node config);
std::string getInclude(YAML::Node config);
std::string getLibraryFiles(YAML::Node config);
std::vector<std::string> getCompile(YAML::Node config);

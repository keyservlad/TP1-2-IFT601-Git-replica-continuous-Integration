/*
   Using YAML-cpp library
   https://github.com/jbeder/yaml-cpp
 */

#include <buildus.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

int build(std::string filePathStr, std::string destinationPathStr) 
{
	// Get BuildUs path
	const auto originalPath = boost::filesystem::current_path();
	auto path = originalPath;

	// Get config
	YAML::Node config = YAML::LoadFile(filePathStr);

	// Check if project name is valid
	if (!config["project"] || config["project"].IsNull()) 
	{
		std::cout << "Error : project name is invalid" << std::endl;
		return 1;
	}

	std::string projectName = config["project"].as<std::string>();

	// Compile intermediate files
	std::string intermediateFiles = compile(config, filePathStr);

	if (intermediateFiles == "Error") 
	{
		return 1;
	}

	std::string commandLine = "g++ " + intermediateFiles + "-o " + boost::filesystem::current_path().append(destinationPathStr).append(projectName).string();

	// Check if there is a library to be added
	if (config["deps_library"]) 
	{
		std::string lib = getLibrary(config);
		commandLine += lib;
	}

	// Build
	int verif = boost::process::system(commandLine);
	if (verif != 0)
	{
		std::cout << "Error : could not build file" << std::endl
			<< "code : " + verif << std::endl;
	}

	return verif;
}

std::string compile(YAML::Node config, std::string filePathStr)
{
	boost::system::error_code code;
	int verif;
	std::string file;
	std::string intermediateFiles;
	std::string include;
	bool checkTime = false;

	// Get paths
	const auto originalPath = boost::filesystem::current_path();
	auto path = originalPath;
	const auto intermediatePath = path.append("intermediate");

	// Get file path without the config file
	boost::filesystem::path filePath(originalPath);
	filePath.append(filePathStr);
	filePath = filePath.parent_path();

	// Create intermediate directory
	if (!boost::filesystem::exists(intermediatePath, code))
	{
		boost::filesystem::create_directory("intermediate", code);
	}

	// Get include if there is any
	if (config["deps_include"])
	{
		include = getInclude(config);
	}

	// Get compile
	std::vector<std::string> toCompile = getCompile(config);
	if (toCompile[0] == "Error")
	{
		std::cout << "Error : could not find compile in the config file" << std::endl;
		return "Error";
	}

	// Compile files.cpp to intermediate files.o
	for (int i = 0; i < toCompile.size(); i++) 
	{
		// File .cpp
		path = filePath;
		path.append(toCompile[i]);
		if (!boost::filesystem::exists(path, code)) 
		{
			std::cout << "Error : " + toCompile[i] + " is missing" << std::endl;
			return "Error";
		}

		auto pathFileO = intermediatePath;
		pathFileO.append(toCompile[i].substr(0, toCompile[i].find(".cpp")) + ".o");
		file = pathFileO.string();

		// File .o
		boost::filesystem::path f(file);

		// Check if files have been modified
		if (boost::filesystem::exists(file)) 
		{
			checkTime = (boost::filesystem::last_write_time(path) == boost::filesystem::last_write_time(f));
		}

		if (!checkTime) 
		{
			verif = boost::process::system("g++ -c -o " + file + " " + path.string() + " " + include);

			// Same time for both files
			boost::filesystem::last_write_time(path, boost::filesystem::last_write_time(f));

			if (verif != 0) 
			{
				std::cout << "Error : could not compile files" << std::endl
					<< "code : " + verif << std::endl;
				return "Error";
			}
		}

		intermediateFiles += file + " ";
	}

	return intermediateFiles;
}

std::string getLibrary(YAML::Node config) 
{
	std::string libDir;
	std::vector<std::string> dir;

	boost::split(dir, config["deps_library"]["var"].as<std::string>(), boost::is_space());

	for (int i = 0; i < dir.size(); i++) 
	{
		std::string path = std::string(getenv(dir[i].c_str()));
		libDir += " -L" + path;
	}

	std::string lib = libDir + getLibraryFiles(config);

	return lib;
}

std::string getInclude(YAML::Node config) 
{
	std::string includeDir;
	std::vector<std::string> dir;

	boost::split(dir, config["deps_include"]["var"].as<std::string>(), boost::is_space());

	for (int i = 0; i < dir.size(); i++) 
	{
		std::string path = std::string(getenv(dir[i].c_str()));
		includeDir += " -I" + path;
	}

	return includeDir;
}

std::string getLibraryFiles(YAML::Node config) 
{
	std::string libraryFiles;

	if (config["deps_library"] || !config["deps_library"].IsNull()) 
	{
		for (int i = 0; i < config["deps_library"]["libs"].size(); i++) 
		{
			libraryFiles += " -l" + config["deps_library"]["libs"][i].as<std::string>();
		}
	}

	return libraryFiles;
}

std::vector<std::string> getCompile(YAML::Node config) 
{

	std::vector<std::string> toCompile;

	if (config["compile"] || !config["compile"].IsNull()) 
	{
		for (int i = 0; i < config["compile"].size(); i++) 
		{
			std::string file = "f" + std::to_string(i + 1);
			toCompile.push_back(config["compile"][i][file].as<std::string>());
		}
	}
	else 
	{
		std::vector<std::string> error;
		error.push_back("Error");
		return error;
	}

	return toCompile;
}

int clean()
{
	boost::system::error_code code;

	// Get intermediate files path
	const auto intermediatePath = boost::filesystem::current_path().append("intermediate");

	if (!boost::filesystem::exists(intermediatePath, code))
	{
		std::cout << "Error : intermediate directory does not exist!" << std::endl;
		return 1;
	}
	else
	{
		boost::filesystem::remove_all(intermediatePath, code);
		return 0;
	}
}
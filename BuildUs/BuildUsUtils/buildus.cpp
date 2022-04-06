#include <buildus.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

// we use the yaml-cpp external library 

int build(std::string filePathStr, std::string destinationPathStr) 
{
	// Get config
	YAML::Node config = YAML::LoadFile(filePathStr);

	// We check that the project name id defined
	if (!config["project"] || config["project"].IsNull()) // SD - devrait etre une constante -2
	{
		std::cout << "Build failed : project name is not valid" << std::endl;
		return 1;
	}

	std::string projectName = config["project"].as<std::string>(); // SD - devrait etre une constante
	const auto intermediatePath = boost::filesystem::current_path().append(destinationPathStr).append(projectName).string();

	// We compile the intermediate files
	std::string intermediateFiles = compile(config, filePathStr);

	if (intermediateFiles == "Error") 
	{
		return 1;
	}

	std::string commandToExecute = "g++ " + intermediateFiles + "-o " + intermediatePath;

	// adds the libraries to the command
	if (config["deps_library"])  // SD - devrait etre une constante
	{
		std::string lib = getLibrary(config);
		commandToExecute += lib;
	}

	// Build
	int commandValidation = boost::process::system(commandToExecute);
	if (commandValidation != 0)
	{
		std::cout << "Error : could not build file" << std::endl
			<< "code : " + commandValidation << std::endl;
	}

	return commandValidation;
}

std::string compile(YAML::Node config, std::string filePathStr)
{
	boost::system::error_code code;
	std::string file;
	std::string intermediateFiles;
	std::string include;
	bool checkTime = false;
	const auto currentPath = boost::filesystem::current_path();
	const auto intermediatePath = currentPath / "intermediate";

	// Get file path without the config file
	boost::filesystem::path filePath(currentPath);
	filePath.append(filePathStr);
	filePath = filePath.parent_path();

	// Create the intermediate directory
	if (!boost::filesystem::exists(intermediatePath, code))
	{
		boost::filesystem::create_directory("intermediate", code);
	}
	if (!boost::filesystem::exists(intermediatePath, code))
	{
		if (code.failed())
		{
			std::cout << "Failed creating the intermediate directory" << std::endl;
		}
	}

	// adds the include if there is one
	if (config["deps_include"]) // SD - devrait etre une constante
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

	// cpp to intermediate o for all the file to compile (loop trough them)
	for (int i = 0; i < toCompile.size(); i++) 
	{
		// File .cpp
		const auto pathToCompile = filePath / toCompile[i];
		if (!boost::filesystem::exists(pathToCompile, code))
		{
			std::cout << "Error : " + toCompile[i] + " is missing" << std::endl;
			return "Error";
		}

		auto pathFileO = intermediatePath;
		pathFileO.append(toCompile[i].substr(0, toCompile[i].find(".cpp")) + ".o");
		file = pathFileO.string();

		// File .o
		boost::filesystem::path fileo(file);

		// Check if files have been modified
		if (boost::filesystem::exists(file))
		{
			checkTime = (boost::filesystem::last_write_time(pathToCompile) == boost::filesystem::last_write_time(fileo));
		}

		if (!checkTime)
		{
			int verif = boost::process::system("g++ -c -o " + file + " " + pathToCompile.string() + " " + include);

			// Same time for both files
			boost::filesystem::last_write_time(pathToCompile, boost::filesystem::last_write_time(fileo));

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

	boost::split(dir, config["deps_library"]["var"].as<std::string>(), boost::is_space()); // SD - devrait etre une constante

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

	boost::split(dir, config["deps_include"]["var"].as<std::string>(), boost::is_space()); // SD - devrait etre une constante

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

	if (config["deps_library"] || !config["deps_library"].IsNull())  // SD - devrait etre une constante
	{
		for (int i = 0; i < config["deps_library"]["libs"].size(); i++)  // SD - devrait etre une constante
		{
			libraryFiles += " -l" + config["deps_library"]["libs"][i].as<std::string>(); // SD - devrait etre une constante
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
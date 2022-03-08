#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <buildus.h>

void help()
{
	std::cout << "usage : " << std::endl
		<< "BuildUs /path/config.buildus /place/to/create/deliverable" << std::endl
		<< "BuildUs clean" << std::endl;
}

int main(int argc, char* argv[]) 
{
	boost::system::error_code code;

	// Convert argv[] to string
	const std::vector<std::string> argvString(argv, argv + argc);

	// Too few args
	if (argc < 2)
	{
		help();
	}

	// 1st arg is name of the file and its relative path
	// 2nd arg is the path to where the deliverable is created
	else if (argc == 3)
	{
		// Get BuildUs path
		const auto originalPath = boost::filesystem::current_path();
		auto path = originalPath;

		// Check if the config file exists, otherwise return error
		path.append(argvString[1]);
		if (!boost::filesystem::exists(path, code))
		{
			std::cout << "Error : " + argvString[1] + " config file does not exist" << std::endl;
			return 1;
		}

		// Check if it ends with .buildus
		std::string str1 = argvString[1].substr(argvString[1].length() - 8, 8);
		if (!(str1 == ".buildus")) 
		{
			std::cout << "Error : unknown config file" << std::endl
				<< "The config file should end with .buildus" << std::endl;
			return 1;
		}

		// Check if the path for the deliverable exists otherwise create it 
		path = originalPath;
		path.append(argvString[2]);
		if (!boost::filesystem::exists(path, code)) 
		{
			boost::filesystem::create_directory(path, code);
		}

		if (build(argvString[1], argvString[2]) != 0)
		{
			std::cout << "Error : build was unsuccessful" << std::endl;
			return 1;
		}
	}

	// Clean
	else if (argc == 2 && argvString[1] == "clean")
	{
		if (clean() == 0)
		{
			std::cout << "Cleaning was successful" << std::endl;
		}
		else
		{
			std::cout << "Error : cleaning was unsuccessful" << std::endl;
			return 1;
		}
	}

	// Invalid command
	else
	{
		help();
	}

	return 0;
}
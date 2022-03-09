#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <buildus.h>

void help()
{
	std::cout << "usage : " << std::endl
		<< "./BuildUs path/config.buildus place/to/create/deliverable" << std::endl
		<< "./BuildUs clean" << std::endl;
}

int main(int argc, char* argv[]) 
{
	boost::system::error_code code;

	// Convert argv[] to string
	const std::vector<std::string> argvString(argv, argv + argc);

	// Not enough arguments, we call the help function
	if (argc < 2)
	{
		help();
	}

	else if (argc == 3)
	{
		const auto currentPath = boost::filesystem::current_path();
		const auto configPath = currentPath / argvString[1];
		boost::system::error_code code;

		// We check that the config file exists
		if (!boost::filesystem::exists(configPath, code))
		{
			std::cout << argvString[1] + " config file does not exist" << std::endl;
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
		const auto deliverablePath = currentPath / argvString[2];
		if (!boost::filesystem::exists(deliverablePath, code))
		{
			boost::filesystem::create_directory(deliverablePath, code);
		}
		
		if (!boost::filesystem::exists(deliverablePath, code))
		{
			if (code.failed())
			{
				std::cout << "Failed creating the deliverable directory" << std::endl;
			}
		}


		// we run the build and return an error in case it fails
		if (build(argvString[1], argvString[2]) != 0)
		{
			std::cout << "Error : build failed..." << std::endl;
			return 1;
		}
	}

	// Clean
	else if (argc == 2 && argvString[1] == "clean")
	{
		// we run the clean function and display if it was successful or not
		if (clean() == 0)
		{
			std::cout << "Clean was successful" << std::endl;
		}
		else
		{
			std::cout << "Error : clean failed" << std::endl;
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
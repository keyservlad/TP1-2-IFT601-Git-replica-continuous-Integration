#include "add.h"
#include "sha1.h"

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>


void Add(std::string pathOfAddedFile)
{
	auto index_path = boost::filesystem::current_path() / ".git" / "index";	// path of index
	auto objects_path = boost::filesystem::current_path() / ".git" / "objects"; // path of the directory objects

	boost::system::error_code code;
	// check that index already exists
	if (!boost::filesystem::exists(index_path, code))
	{
		std::cout << "File index is missing, make sure you run the init command" << std::endl;
		return;
	}

	if (!boost::filesystem::exists(objects_path))  // peut lancer une exception
	{
		std::cout << "The objects directory is missing, make sure you run the init command" << std::endl;
		return;
	}

	auto pathAddedFile = boost::filesystem::current_path() / pathOfAddedFile; // path of the added file
	// we check that the file actually exists
	if (!boost::filesystem::exists(pathAddedFile, code))
	{
		std::cout << "The file you are trying to add doesn't exist" << std::endl;
		return;
	}

	// we generate the sha1 of the file path using the string path
	string sha1 = Sha1Generator(pathOfAddedFile);

	// TODO write sha1 + ' ' + path into index (and check that the file is not already here)
	// TODO create file in the /objects with infos and named after sha1

}
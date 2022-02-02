#include <iostream>
#include <fstream>
#include <cerrno>
#include <boost/filesystem.hpp>
#include <ctime>

#include <init.h>
#include <add.h>
#include <commit.h>

void Help()
{
	std::cout << "User manual: gitus <commande> [<arguments>]" << std::endl
		<< "Here are the different gitus commands used in this project:" << std::endl
		<< "init: Creating an empty Git directory " << std::endl
		<< "add: Adding the contents of a file to the staging area" << std::endl
		<< "commit:	Save changes to directory" << std::endl;
}


std::vector<std::string> argvString;

int main(int argc, char* argv[])
{
	// Enregistrement des arguments sous forme d'une chaine de caractères
	argvString = std::vector<std::string>(argv, argv + argc);

	// Au cas de demande d'aide ou de mal entrer les arguments
	if (argc < 2 || argvString[1] == "--help")
		Help();

	// init
	else if (argvString[1] == "init")
	{
		if (argc == 2)
			Init();
		else
			std::cout << "How to use: gitus init" << std::endl;
	}

	// add
	else if (argvString[1] == "add")
	{
		if (argc == 3)
			Add(argvString[2]);
		else
			std::cout << "How to use: gitus add <pathspec>" << std::endl;
	}
	else if (argvString[1] == "commit")
	{
		if (argc == 4)
			Commit(argvString[2], argvString[3]);
		else
			std::cout << "How to use: gitus commit <msg> <author>" << std::endl;
	}

	// Not a valid command
	else
		Help();

    return 42;
}




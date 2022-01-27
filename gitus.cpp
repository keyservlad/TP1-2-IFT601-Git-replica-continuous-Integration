#include <iostream>
#include <fstream>
#include <cerrno>

#include <init.h>
#include <dummy.h>


void Help()
{
	std::cout << "Guide d'utilisation: gitus <commande> [<arguments>]" << std::endl
		<< "Voici les différentes commande gitus utilisées dans ce projet :" << std::endl
		<< "init: Création d'une répertoire Git vide " << std::endl
		<< "add: Ajout du contenu d'un fichier dans <index>" << std::endl
		<< "commit:	Enregistre les modifications dans la répertoire" << std::endl;
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
			std::cout << "usage: gitus init" << std::endl;
	}

	// Not a valid command
	else
		Help();

    return 42;
}




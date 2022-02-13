/** AB - Commentaires
*  
*       Manque beaucoup de constantes -5
*       Accolades   -2
*       Exception possible -5
*       non gestion de git add --help -2
*       mauvaise génération du SHA1 -5
*       constance de la nomenclature -2 (snake_case et camelCase)
*       Commentaire faible -2
*       Manque commentaire -2
*       Nomenclature x2 -4
*       Effet de bord -2
*       Mauvaise gestion de la création d'arbre -5
*       Utilisation du message et auteur pour la génération de l'arbre? -5
*       Qualité générale du code faible -5
*       
*       Tests: 15/25
* 
* 
*       Total:
*           44/100
*/


#include <iostream>
#include <fstream>
#include <cerrno>
#include <boost/filesystem.hpp> // AB - include inutile ici
#include <ctime> // AB - include inutile ici

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


std::vector<std::string> argvString; // AB - pourquoi une variable globale?? -2

int main(int argc, char* argv[])
{
	// Enregistrement des arguments sous forme d'une chaine de caractères
	argvString = std::vector<std::string>(argv, argv + argc);

	// Au cas de demande d'aide ou de mal entrer les arguments
	if (argc < 2 || argvString[1] == "--help")
		Help();

	// init
	else if (argvString[1] == "init") // AB - contante?
	{
		if (argc == 2)
			Init();
		else
			std::cout << "How to use: gitus init" << std::endl;
	}

	// add
	else if (argvString[1] == "add") // AB - idem
	{
		if (argc == 3) // AB - et gitus add --help? -2
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

    return 42; // AB - très mauvaise pratique. Tout ce qui n'est pas 0 signifie qu'il y a eu une erreur. -2
}




#include "init.h"
#include <iostream>
#include <boost/filesystem.hpp>


void Init() {
	const auto current_path = boost::filesystem::current_path();	// ou sommes nous?

	// l'opéreteur "/" sert à concaténer un chemin (path -- style boost) avec un chemin ou bien une chaine de caractères
	const auto gitFolder = current_path / ".git";	// ajouter un fichier de la manière décidée par la plateforme
													// comme ca, on aura jamais de problème de plateforme

	if (!boost::filesystem::exists(gitFolder))  // peut lancer une exception
	{
		// si le répertoire n'existe pas, on le crée
		boost::filesystem::create_directory(gitFolder); // exception possible

	}
	else
		std::cout << "<.git> is already created !" << std::endl;

	boost::system::error_code code;
	if (!boost::filesystem::exists(gitFolder, code)) // ne lance pas d'exception .. c'est attrapé par le code
	{
		if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	const auto objFolder = gitFolder / "objects";

	// création du dossier objets dans ./git 
	if (!boost::filesystem::exists(objFolder))  // peut lancer une exception
	{
		// si le répertoire n'existe pas, on le crée
		boost::filesystem::create_directory(objFolder); // exception possible

	}
	else
		std::cout << "<.git/objects> is already created !" << std::endl;

	if (!boost::filesystem::exists(gitFolder, code)) // ne lance pas d'exception .. c'est attrapé par le code
	{
		if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	const auto indFilePath = gitFolder / "index";

	// Création du fichier index dans la répertoire ./git
	if (!boost::filesystem::exists(indFilePath))
	{
		std::ofstream outputFile(indFilePath.c_str());
		outputFile.close();
	}
	else
		std::cout << "The index file is already created !" << std::endl;

	if (!boost::filesystem::exists(indFilePath, code)) // ne lance pas d'exception .. c'est attrapé par le code
	{
		if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	const auto headFilePath = gitFolder / "HEAD";

	// Create empty .git/HEAD file
	if (!boost::filesystem::exists(headFilePath))
	{
		std::ofstream outputFile(headFilePath.c_str());
		outputFile.close();
	}
	else
		std::cout << "The HEAD file is already created !" << std::endl;

	if (!boost::filesystem::exists(headFilePath, code)) // ne lance pas d'exception .. c'est attrapé par le code
	{
		if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	std::cout << "The initialisation of the repository has been successfully established ! :)" << gitFolder << std::endl;
}


#include "init.h"
#include <iostream>
#include <boost/filesystem.hpp>


void Init() {
	auto current_path = boost::filesystem::current_path();	// ou sommes nous?

	// l'op�reteur "/" sert � concat�ner un chemin (path -- style boost) avec un chemin ou bien une chaine de caract�res
	const auto gitFolder = current_path / ".git";	// ajouter un fichier de la mani�re d�cid�e par la plateforme
													// comme ca, on aura jamais de probl�me de plateforme

	if (!boost::filesystem::exists(gitFolder))  // peut lancer une exception
	{
		// si le r�pertoire n'existe pas, on le cr�e
		boost::filesystem::create_directory(gitFolder); // exception possible

	}
	else
		std::cout << "La r�pertoire .git est d�j� cr��e" << std::endl;

	boost::system::error_code code;
	if (!boost::filesystem::exists(gitFolder, code)) // ne lance pas d'exception .. c'est attrap� par le code
	{
		if (code.failed()) // et ensuite, on v�rifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	auto objFolder = gitFolder / "objects";

	// cr�ation du dossier objets dans ./git 
	if (!boost::filesystem::exists(objFolder))  // peut lancer une exception
	{
		// si le r�pertoire n'existe pas, on le cr�e
		boost::filesystem::create_directory(objFolder); // exception possible

	}
	else
		std::cout << "Le r�pertoire .git/objects est d�j� cr��" << std::endl;

	if (!boost::filesystem::exists(gitFolder, code)) // ne lance pas d'exception .. c'est attrap� par le code
	{
		if (code.failed()) // et ensuite, on v�rifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	auto indFilePath = gitFolder / "index";

	// Cr�ation du fichier index dans la r�pertoire ./git
	if (!boost::filesystem::exists(indFilePath))
	{
		std::ofstream outputFile(indFilePath.c_str());
		outputFile.close();
	}
	else
		std::cout << "Le fichier index est d�j� cr��" << std::endl;

	if (!boost::filesystem::exists(indFilePath, code)) // ne lance pas d'exception .. c'est attrap� par le code
	{
		if (code.failed()) // et ensuite, on v�rifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	auto headFilePath = gitFolder / "HEAD";

	// Create empty .git/HEAD file
	if (!boost::filesystem::exists(headFilePath))
	{
		std::ofstream outputFile(headFilePath.c_str());
		outputFile.close();
	}
	else
		std::cout << "Le fichier HEAD est d�j� cr��" << std::endl;

	if (!boost::filesystem::exists(headFilePath, code)) // ne lance pas d'exception .. c'est attrap� par le code
	{
		if (code.failed()) // et ensuite, on v�rifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	std::cout << "L'initialisation du r�pertoire GIT est termin� avec succ�s ! :)" << gitFolder << std::endl;
}


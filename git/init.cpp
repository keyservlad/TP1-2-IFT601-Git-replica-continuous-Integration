#include "init.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/uuid/detail/sha1.hpp>


void Init() {
	auto current_path = boost::filesystem::current_path();	// ou sommes nous?

	// l'opéreteur "/" sert à concaténer un chemin (path -- style boost) avec un chemin ou bien une chaine de caractères
	const auto gitFolder = current_path / ".git";	// ajouter un fichier de la manière décidée par la plateforme
													// comme ca, on aura jamais de problème de plateforme

	if (!boost::filesystem::exists(gitFolder))  // peut lancer une exception
	{
		// si le répertoire n'existe pas, on le crée
		boost::filesystem::create_directory(gitFolder); // exception possible

	}

	boost::system::error_code code;
	if (!boost::filesystem::exists(gitFolder, code)) // ne lance pas d'exception .. c'est attrapé par le code
	{
		if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}
}
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

	// l'op�reteur "/" sert � concat�ner un chemin (path -- style boost) avec un chemin ou bien une chaine de caract�res
	const auto gitFolder = current_path / ".git";	// ajouter un fichier de la mani�re d�cid�e par la plateforme
													// comme ca, on aura jamais de probl�me de plateforme

	if (!boost::filesystem::exists(gitFolder))  // peut lancer une exception
	{
		// si le r�pertoire n'existe pas, on le cr�e
		boost::filesystem::create_directory(gitFolder); // exception possible

	}

	boost::system::error_code code;
	if (!boost::filesystem::exists(gitFolder, code)) // ne lance pas d'exception .. c'est attrap� par le code
	{
		if (code.failed()) // et ensuite, on v�rifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}
}
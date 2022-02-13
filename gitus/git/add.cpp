#include "add.h"
#include "sha1.h"

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp> // AB - ?
#include <boost/iostreams/filtering_streambuf.hpp>
#include <sstream>


namespace fs = boost::filesystem;

// boolean et return sont utiles pour les tests
bool Add(string pathOfAddedFile)
{
    // AB - j'ai déja vu ".git" a quelque part...ca aurait pu etre une belle constante...

	const auto index_path = boost::filesystem::current_path() / ".git" / "index";	// path of index
	const auto objects_path = boost::filesystem::current_path() / ".git" / "objects"; // path of the directory objects
    // AB - on regarde ici un snake_case


	boost::system::error_code code;

	// Nous vérifions que la répertoire est déjà initialisée
	if (!boost::filesystem::exists(index_path, code))
	{
		cout << "File index is missing, make sure you run the init command" << endl;
		return false;
	}

	if (!boost::filesystem::exists(objects_path))  // peut lancer une exception
	{
		cout << "The objects directory is missing, make sure you run the init command" << endl;
		return false;
	}

	// Chemin du fichier à ajouter
	const auto pathAddedFile = boost::filesystem::current_path() / pathOfAddedFile; 
    // AB - ici c'est du camelCase...constance de la nomenclature -2
	
	// Nous vérifions que le fichier existe déjà 
	if (!boost::filesystem::exists(pathAddedFile, code))
	{
		cout << "The file you are trying to add doesn't exist" << endl;
		return false;
	}

	// Nous générons le sha1 du fichier à ajouter
	const string sha1 = Sha1Generator(pathOfAddedFile);

	string index;
	const string index_Content = sha1 + " " + pathOfAddedFile;
	const string directory = sha1.substr(0, 2);
	const string file = sha1.substr(2);
	const string added_File = fs::path(pathOfAddedFile).filename().string();

	// Extraction du contenu du fichier
	const ifstream input_Added_File(pathOfAddedFile);
	stringstream content_Stream;
	content_Stream << input_Added_File.rdbuf();
	string added_File_Content = content_Stream.str();

	// Récupération du chemin actuel et création du dossier
    
    // AB - francais pour les noms de variables maintenant?
	const auto chemin = boost::filesystem::current_path() / ".git" / "objects" / directory;	// chemin du dossier
	if (!fs::exists(chemin, code))
		fs::create_directory(chemin);

	// Création du fichier avec son contenu
	const auto chemin2 = boost::filesystem::current_path() / ".git" / "objects" / directory / file;	// chemin du fichier
	if (fs::exists(chemin2, code))
	{
        // AB - ouf...si on est rendu là, c'est que ca va très mal pour votre dépot...
		cout << "The file " << chemin2 << " is already existing !" << endl
			<< "Overwritting ..." << endl;
	}

	ofstream output_File(chemin2.c_str());
	output_File << added_File << " " << added_File_Content.size() << " " << added_File_Content << endl;
	output_File.close();

	fs::ofstream index_Output(index_path.c_str(), ios::app);
	fs::ifstream index_Input(index_path.c_str());

	// Si index est vide, nous ajouttons le contenu
	if (fs::is_empty(index_path))
	{
		index_Output << index_Content << endl;
	}
	// Si non nous vérifions si le contenu est déjà écrit
	else
	{
		bool verify = true;

		while (getline(index_Input, index)) // AB - index est un très mauvais nom ici...
		{
			if (index.find(index_Content) != string::npos)
				verify = false;
		}

		if (verify == true) // AB - if (verify) ? ca serait pas mieu?
			index_Output << index_Content << endl;
	}
	index_Input.close();
	index_Output.close();

	cout << "The file <" <<  fs::path(pathOfAddedFile).filename().string() << "> has been added successfully ! :) " << endl;
	return true;
}
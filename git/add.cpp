#include "add.h"
#include "sha1.h"

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <sstream>


namespace fs = boost::filesystem;

fs::path Creat_Sha1_Dir_File(string sha1, string pathOfAddedFile)
{
	boost::system::error_code code;
	string directory = sha1.substr(0, 2);
	string file = sha1.substr(2);
	string added_File = fs::path(pathOfAddedFile).filename().string();

	// Extraire le contenu du fichier
	ifstream input_Added_File(pathOfAddedFile);
	std::stringstream content_Stream;
	content_Stream << input_Added_File.rdbuf();
	string added_File_Content = content_Stream.str();

	// Récupération du chemin actuel et création du dossier
	auto chemin = boost::filesystem::current_path() / ".git" / "objects" / directory;	// chemin du dossier
	if (!fs::exists(chemin, code))
		fs::create_directory(chemin);

	// Création du fichier avec son contenu
	auto chemin2 = boost::filesystem::current_path() / ".git" / "objects" / directory / file;	// chemin du dossier
	if (fs::exists(chemin2, code))
	{
		cout << "Le fichier " << chemin2 << " existe déjà !" << endl
			<< "Ecrasement de son contenu ..." << endl;
	}
	std::ofstream output_File(chemin2.c_str());
	output_File << added_File << " " << added_File_Content.size() << " " << added_File_Content << endl;
	output_File.close();

	return chemin2;
}


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
	// Done !

	string index;
	string index_Content = sha1 + " " + pathOfAddedFile;

	fs::path File_Path_sha1 = Creat_Sha1_Dir_File(sha1, pathOfAddedFile);

	fs::ifstream index_Input(index_path.c_str());
	fs::ofstream index_Output(index_path.c_str(), ios::app);

	// Si index est vide, nous ajouttons le contenu
	if (fs::is_empty(index_path))
	{
		index_Output << index_Content << endl;
	}
	// Si non nous vérifions si le contenu est déjà écrit
	else
	{
		bool verify = true;

		while (std::getline(index_Input, index))
		{
			if (index.find(index_Content) != string::npos)
				verify = false;
		}

		if (verify == true)
			index_Output << index_Content << endl;
	}
	index_Input.close();
	index_Output.close();

	std::cout << "Le fichier a été ajouté avec succès !! :) " << fs::path(pathOfAddedFile).filename().string() << std::endl;
	return;


}
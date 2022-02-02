#include "sha1.h"
#include <boost/uuid/detail/sha1.hpp>
#include <sstream>
#include <fstream>
#include <iomanip>


using namespace boost::uuids::detail;



string Sha1Generator(string path)
{
	sha1 sha;
	sha.process_bytes(path.c_str(), path.length());

	unsigned int hash[5];
	sha.get_digest(hash); // get 5 uint (32 bits -- that can be converted into 8 hex characters) that, combined, maked for the 40 characters
	// This is done by just transforming the uints into hexadecimal values // 

	stringstream stream;
	for (int i = 0; i < 5; ++i)
	{
		stream << hex << hash[i] << setfill('0') << setw(8);
	}
	string result = stream.str();
	return result;
}

// Création du sha1 pour la fonction Add()
string Sha1_Add_Generator(string path)
{
	// fichier de l'input
	ifstream fichier(path);

	string contenu{ std::istreambuf_iterator<char>(fichier),
					std::istreambuf_iterator<char>() };

	// Longuer du ficher
	int longueur = contenu.length();
	string TheHeader = "sha1 " + std::to_string(longueur);
	contenu = TheHeader + contenu;

	sha1 sha;
	sha.process_bytes(contenu.c_str(), contenu.length());

	unsigned int hash[5];
	sha.get_digest(hash);

	std::stringstream stream;
	for (int i = 0; i < 5; ++i)
		stream << std::hex << hash[i];

	return stream.str();
}
#include "sha1.h"
#include <boost/uuid/detail/sha1.hpp>
#include <sstream>
#include <fstream>


using namespace boost::uuids::detail;



string Sha1Generator(string path)
{
	ifstream file(path);	// pas besoin de gerer l'acces
								// mais il faut s'assurer que le fichier existe

	// lecture automatique du contenu et insertion dans une string
	string content{ std::istreambuf_iterator<char>(file),
					std::istreambuf_iterator<char>() };


	sha1 sha;
	sha.process_bytes(content.c_str(), content.length());

	// bof... would prefer array...but the digest does not support it
	unsigned int hash[5];
	sha.get_digest(hash); // get 5 uint (32 bits -- that can be converted into 8 hex characters) that, combined, maked for the 40 characters
	// This is done by just transforming the uints into hexadecimal values // 


	std::stringstream stream;

	std::string result;
	// there is something missing here...but what?
	for (int i = 0; i < 5; ++i)
	{
		stream << std::hex << hash[i];
	}
	result += stream.str();
	return result;
}
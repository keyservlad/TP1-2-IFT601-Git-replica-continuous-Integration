#include "sha1.h"
#include <boost/uuid/detail/sha1.hpp>
#include <sstream>
#include <fstream>
#include <iomanip>


using namespace boost::uuids::detail;



string Sha1Generator(string path)
{
	ifstream file(path);

	// lecture automatique du contenu et insertion dans une string
	string content{ istreambuf_iterator<char>(file),
					istreambuf_iterator<char>() };


	sha1 sha;
	sha.process_bytes(content.c_str(), content.length());

	unsigned int hash[5];
	sha.get_digest(hash); // get 5 uint (32 bits -- that can be converted into 8 hex characters) that, combined, maked for the 40 characters
	// This is done by just transforming the uints into hexadecimal values // 

	stringstream stream;

	string result;
	for (int i = 0; i < 5; ++i)
	{
		stream << hex << hash[i] << setfill('0') << setw(8);
	}
	result += stream.str();
	return result;
}

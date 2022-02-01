#define CATCH_CONFIG_MAIN

// RTFM catch2:
// https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#top

#include "catch.hpp"
#include <init.h>
#include <add.h>
#include <sha1.h>
#include <commit.h>
#include <boost/filesystem.hpp>
#include <sha1.cpp>

namespace fs = boost::filesystem;

// On cherche le path de git, on le supprime s'il existe déjà puis on init un nouveau dépot
void initializeGitus(boost::system::error_code code)
{
	const auto gitFolder = fs::current_path() / ".git";

	if (fs::exists(gitFolder, code))
	{
		fs::remove_all(gitFolder, code);

		if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
		{
			std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
		}
	}

	Init();
}

// On créé un fichier test avec du contenu pour simuler une modification d'un utilisateur 
void createTestFile(boost::system::error_code code)
{
	const auto testFile = fs::current_path() / "test.txt";

	std::ofstream outputFile(testFile.c_str());
	outputFile << "fichier test" << endl;

	outputFile.close();
}

TEST_CASE("init")
{
	boost::system::error_code code;

	// on créé les variables des chemins des fichiers et dossiers créés par init
	const auto gitFolder = fs::current_path() / ".git";
	const auto objectsFolder = fs::current_path() / ".git" / "objects";
	const auto indexPath = fs::current_path() / ".git" / "index";
	const auto headPath = fs::current_path() / ".git" / "HEAD";

	// on initialise un nouveau depot
	initializeGitus(code);

	// on teste les fichiers et dossiers
	REQUIRE(fs::exists(gitFolder, code));
	REQUIRE(fs::exists(objectsFolder, code));
	REQUIRE(fs::exists(indexPath, code));
	REQUIRE(fs::exists(headPath, code));
}

TEST_CASE("init-twice")
{
	boost::system::error_code code;

	// on créé les variables des chemins des fichiers et dossiers créés par init
	const auto gitFolder = fs::current_path() / ".git";
	const auto objectsFolder = fs::current_path() / ".git" / "objects";
	const auto indexPath = fs::current_path() / ".git" / "index";
	const auto headPath = fs::current_path() / ".git" / "HEAD";

	// on initialise un nouveau depot
	initializeGitus(code);
	// on appelle une seconde fois init
	Init();

	// on teste les fichiers et dossiers
	REQUIRE(fs::exists(gitFolder, code));
	REQUIRE(fs::exists(objectsFolder, code));
	REQUIRE(fs::exists(indexPath, code));
	REQUIRE(fs::exists(headPath, code));
}


TEST_CASE("add")
{
	boost::system::error_code code;

	// on créé les variables des chemins des fichiers et dossiers créés par init
	const auto gitFolder = fs::current_path() / ".git";
	const auto testFile = fs::current_path() / "test.txt";
	const auto indexPath = fs::current_path() / ".git" / "index";

	// on commence par un init
	initializeGitus(code);

	// On créé le fichier de test qu'on va add
	createTestFile(code);
	REQUIRE(fs::exists(testFile, code));

	// on vérifie que index existe et est vide
	REQUIRE(fs::exists(indexPath, code));
	REQUIRE(fs::is_empty(indexPath, code));



	// on créé le sha1 du fichier et on vérifie son path
	const string sha1 = Sha1Generator("test.txt");
	const string sha1DirectoryName = sha1.substr(0, 2);
	const string sha1FileName = sha1.substr(2);
	auto const sha1Path = fs::current_path() / ".git" / "objects" / sha1DirectoryName / sha1FileName;
	REQUIRE(!fs::exists(sha1Path, code));

	// Ensuite on teste le add
	REQUIRE(Add("test.txt"));

	// On récupère le contenu de l'index et on le vérifie avec ce que ca devrait être
	fs::ifstream indexInputFile(indexPath.c_str());
	stringstream indexContentStream;
	indexContentStream << indexInputFile.rdbuf();
	indexInputFile.close();

	const string indexContent = indexContentStream.str();
	const string indexContentInputed = "test.txt " + sha1 + "\n";

	REQUIRE(indexContent == indexContentInputed);

	// on vérifie que le sha1 existe dans objects
	REQUIRE(fs::exists(sha1Path, code));

	// on verifie son contenu de la même maniere que dans l'index
	fs::ifstream sha1InputFile(sha1Path.c_str());
	stringstream sha1ContentStream;
	sha1ContentStream << sha1InputFile.rdbuf();
	sha1InputFile.close();

	const string sha1Content = sha1ContentStream.str();
	const string sha1ContentInputed = "test.txt " + std::to_string(7) + " fichier test\n\n";

	REQUIRE(sha1Content == sha1ContentInputed);
}


TEST_CASE("add-unknown-file")
{
	boost::system::error_code code;

	initializeGitus(code);

	// On vérifie qu'aucun fichier n'est ajouté et qu'une erreur est renvoyée
	REQUIRE(!Add("unknown.txt"));
}

TEST_CASE("add-without-init")
{
	boost::system::error_code code;

	const auto gitFolder = fs::current_path() / ".git";
	const auto testFile = fs::current_path() / "test.txt";

	// si le dossier git existe, on le supprime pour simuler un appel sans le init
	if (fs::exists(gitFolder, code))
	{
		fs::remove_all(gitFolder, code);
	}
	REQUIRE(!fs::exists(gitFolder, code));

	// on créé le fichier de test comme dans le add
	createTestFile(code);
	REQUIRE(fs::exists(testFile, code));

	// On vérifie qu'aucun fichier n'est ajouté et qu'une erreur est renvoyée
	REQUIRE(!Add("test.txt"));
}


TEST_CASE("commit")
{
}
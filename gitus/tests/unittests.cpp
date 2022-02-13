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
	const string sha1 = Sha1Generator("test.txt"); // AB - ca aurait été mieu de savoir à l'avance le sha1 généré et de le tester avec celui-ci
	const string sha1_Directory = sha1.substr(0, 2);
	const string sha1_File = sha1.substr(2);
	auto const sha1_Path = fs::current_path() / ".git" / "objects" / sha1_Directory / sha1_File;
	REQUIRE(!fs::exists(sha1_Path, code));

	// Ensuite on teste le add
	REQUIRE(Add("test.txt"));

	// On récupère le contenu de l'index et on le vérifie avec ce que ca devrait être
	fs::ifstream index_Input(indexPath.c_str());
	stringstream indexContentStream;
	indexContentStream << index_Input.rdbuf();
	index_Input.close();

	const string indexContent = indexContentStream.str();
	const string indexContentInputed = sha1 + " test.txt" + "\n";

	REQUIRE(indexContent == indexContentInputed);

	// on vérifie que le sha1 existe dans objects
	REQUIRE(fs::exists(sha1_Path, code));

	// on verifie son contenu de la même maniere que dans l'index
	fs::ifstream sha1InputFile(sha1_Path.c_str());
	stringstream sha1ContentStream;
	sha1ContentStream << sha1InputFile.rdbuf();
	sha1InputFile.close();

	const string sha1Content = sha1ContentStream.str();
	const string sha1ContentInputed = "test.txt " + std::to_string(13) + " fichier test\n\n";

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
    // AB - ce test est beaucoup moins interessant que les autres...
	boost::system::error_code code;

	// on créé les variables des chemins des fichiers et dossiers créés par init
	const auto gitFolder = fs::current_path() / ".git";
	const auto objects_Folder = fs::current_path() / ".git" / "objects";
	const auto index_Path = fs::current_path() / ".git" / "index";
	const auto head_Path = fs::current_path() / ".git" / "HEAD";

	const string msg = "message";
	const string author = "alice";

	Init();

	// On créé le fichier de test puis on fait le add
	createTestFile(code);
	Add("test.txt");

	// On fait le commit et vérifie que ca se passe bien
	REQUIRE(Commit(msg, author));

	// On vérifie que index existe toujours et qu'il est vide
	REQUIRE(fs::exists(index_Path, code));
	REQUIRE(fs::is_empty(index_Path, code));

	//on génère le sha1 et on le stocke dans une constante
	const string sha1 = Sha1Generator("commit" + msg + author);

	const string tree = Creat_Tree(index_Path.string(), "tree" + msg + author);

	// on vérifie que head existe et qu'il n'est pas vide
	REQUIRE(fs::exists(index_Path, code));
	REQUIRE(!fs::is_empty(head_Path, code));

	// On récupère le contenu du HEAD
	fs::ifstream Head_Input(head_Path.c_str());
	stringstream Head_Content;
	Head_Content << Head_Input.rdbuf();
	Head_Input.close();

	const string Head_Content_File = Head_Content.str();

	// Obtenir le sha1 généré et son chemain
	const string sha1_Directory = sha1.substr(0, 2);
	const string sha1_File = sha1.substr(2);
	auto const sha1_Path = fs::current_path().append(".git").append("objects").append(sha1_Directory).append(sha1_File);

	fs::ifstream sha1_Input(sha1_Path.c_str());
	stringstream sha1_Content;
	sha1_Content << sha1_Input.rdbuf();
	sha1_Input.close();

	const string sha1FileContent = sha1_Content.str();

    // AB - hum?
}
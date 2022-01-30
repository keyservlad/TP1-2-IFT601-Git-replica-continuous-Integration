#include "commit.h"
#include "sha1.h"
#include <boost/filesystem.hpp>
#include <ctime>

namespace fs = boost::filesystem;


void Commit(std::string message, std::string author)
{
    std::string parent = "\0";
    std::string sha1 = Sha1Generator("commit" + message + author);
    boost::system::error_code code;
    const string dir_name = sha1.substr(0, 2);
    const string file_name = sha1.substr(2);
    auto currentPath = boost::filesystem::current_path();
    auto const myPath = currentPath / ".git" / "objects" / dir_name;

    boost::filesystem::create_directory(myPath, code);
    if (!boost::filesystem::exists(myPath, code)) // ne lance pas d'exception .. c'est attrapé par le code
    {
        if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
        {
            std::cout << "Something bad happened but we didn't throw anything...so thats good :)" << std::endl;
        }
    }
    boost::filesystem::ofstream(currentPath / file_name);
    std::string file = currentPath.string();

    time_t myTime = time(0);
    char* d_time = ctime(&myTime);

    fs::path myHead{ ".git/HEAD" };
    fs::path myObjects{ file };

    fs::ifstream i_head{ myHead };

    if (!fs::is_empty(myHead))
        i_head >> parent;

    fs::ofstream o_head(myHead);
    fs::ofstream ofs_obj(myObjects);

    std::string tree = Creat_Tree(".git/index", "tree" + message + author); // Create tree

    // Ajouter les fichiers parents déjà présents
    if (parent != "\0")
    {
        std::string myLine;
        std::string Tree_parent;
       
        std::string Path_parent = parent.substr(0, 2) + "/" + parent.substr(2);
        std::string Path_tree = tree.substr(0, 2) + "/" + tree.substr(2);

        auto thisPath = fs::current_path() / ".git/objects/" / Path_parent;

        // Lire parent pour obtenir son arbre
        fs::path p_Parent{ thisPath };
        fs::ifstream ifs{ p_Parent };

        while (std::getline(ifs, myLine))
            if (myLine.substr(0, 4).find("tree") != std::string::npos)
                Tree_parent = myLine.substr(5, 2) + "/" + myLine.substr(7);

        std::string parent_Line;
        std::string child_Line;

        auto obj_Path = fs::current_path() / ".git/objects/";

        fs::path t_Parent{ obj_Path / Tree_parent };
        fs::path t_Child{ obj_Path / Path_tree };

        fs::ifstream ifs_2{ t_Parent };
        fs::ifstream ifs_3{ t_Child };

        fs::ofstream ofs{ t_Child, ios::app };

        // On boucle alors que le fichier parent a encore une ligne
        while (std::getline(ifs_2, parent_Line))
        {
            while (std::getline(ifs_3, child_Line))
            {
                if (child_Line.find(parent_Line) == std::string::npos)
                    ofs << parent_Line << endl;
            }
        }
    }
         

    // Faire le commit
    ofs_obj << "parent " << parent << endl << "tree " << tree << endl << d_time << endl << "author " << author << endl << "author " << endl << message << endl;
    o_head << sha1 << endl; // mettre à jour le HEAD

    o_head.close();
    ofs_obj.close();

    std::cout << "Commiter avec succes !!" << std::endl;
}


/*
* Cette fonction est utilisée pour vérifier si une ligne est déjà écrite ou pas
* Elle prend comme parametres: le chemain et le text à vérifier
* Elle retourne 1 si le text est présent, 0 sinon
*/

bool Verif_File(std::string path, std::string txt)
{
    fs::path chemain{ path };
    fs::ifstream ifs{ chemain };
    // Si le chemain est vide ne rien faire 
    if (fs::is_empty(chemain))
        return 0;
    else
    {
        std::string line;
        while (std::getline(ifs, line))
            if (line.find(txt) != std::string::npos)
                return 1;
    }
    return 0;
}

/*
* Cette fonction assure la création de l'arbre
* Elle prend comme parameres: le chemain et la chaine de caractères
* Elle génère finalement l'arbre sha1
*/
std::string Creat_Tree(std::string chemain, std::string txt)
{
    boost::system::error_code code;
    std::string myData;
    std::string myTxt = "tree" + txt;
    std::string sha1 = Sha1Generator(myTxt);
    std::string myFile = "";

    try
    {
        myFile = Sha1Generator(sha1);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    // Si le chemin se trouve dans un autre sous-dossier
    if (chemain != ".git/index")
    {
        fs::path index_path{ ".git/index" };
        fs::ifstream ifs_index{ index_path };

        fs::path myObjects{ myFile };
        fs::ofstream myTree(myObjects, ios::app);

        // Lire les lignes de l'index
        while (std::getline(ifs_index, myData))
        {
            std::string myDir = myData.substr(chemain.length(), myData.find("/") - 2);

            if (myData.substr(0, chemain.length()) == chemain) // Vérifier le chemin du sous-dossier
            {
                if (Verif_File(myFile, myData) == false) // Vérifiez que la ligne n'est pas déjà écrite
                {
                    if (fs::is_directory((chemain + myDir), code)) // Vérifiez si la ligne contient un dossier
                    {
                        std::string subtree = Creat_Tree(chemain + myDir, "subtree" + myDir);  // Créer une arborescence pour le dossier
                        myTree << "tree " << myDir.substr(1) << " " << subtree << endl;
                    }
                    else
                        myTree << "blob " << myData.substr(chemain.length() + 1) << endl;
                }
            }
        }
        myTree.close();
    }
    else
    {
        fs::path index_path{ chemain };
        fs::ifstream ifs_index{ index_path };

        fs::path myObjects{ myFile };
        fs::ofstream myTree(myObjects, ios::app);

        while (std::getline(ifs_index, myData))
        {
            std::string myDir = myData.substr(0, myData.find("/"));
            if (Verif_File(myFile, myDir) == 0)
            {
                if (fs::is_directory(myDir))
                {
                    std::string subtree = Creat_Tree(myDir, "subtree" + myDir);
                    myTree << "tree " << myDir << " " << subtree << endl;
                }
                else
                    myTree << "blob " << myData << endl;
            }
        }
        myTree.close();
    }

    fs::path index{ ".git/index" };
    fs::ofstream ofs_index(index);      // Effacer le fichier d'index

    return sha1;
}


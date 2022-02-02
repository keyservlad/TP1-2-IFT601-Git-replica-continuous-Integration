#include "commit.h"
#include "sha1.h"
#include <boost/filesystem.hpp>
#include <ctime>

namespace fs = boost::filesystem;


bool Commit(string message, string author)
{
    string parent = "\0";
    const string sha1 = Sha1Generator("commit" + message + author);
    const string file = Dir_creation(sha1);

    const time_t myTime = time(0);
    const char* d_time = ctime(&myTime);

    const fs::path myHead{ ".git/HEAD" };
    const fs::path myObjects{ file };

    fs::ifstream i_head{ myHead };

    if (!fs::is_empty(myHead))
        i_head >> parent;

    fs::ofstream o_head(myHead);
    fs::ofstream ofs_obj(myObjects);

    const string tree = Creat_Tree(".git/index", "tree" + message + author); // Create tree

    // Ajouter les fichiers parents déjà présents
    if (parent != "\0")
    {
        string myLine;
        string Tree_parent;
       
        const string Path_parent = parent.substr(0, 2) + "/" + parent.substr(2);
        const string Path_tree = tree.substr(0, 2) + "/" + tree.substr(2);

        const auto thisPath = fs::current_path() / ".git/objects/" / Path_parent;

        // Lire parent pour obtenir son arbre
        const fs::path p_Parent{ thisPath };
        fs::ifstream ifs{ p_Parent };

        while (getline(ifs, myLine))
            if (myLine.substr(0, 4).find("tree") != string::npos)
                Tree_parent = myLine.substr(5, 2) + "/" + myLine.substr(7);

        string parent_Line;
        string child_Line;

        auto obj_Path = fs::current_path() / ".git/objects/";

        const fs::path t_Parent{ obj_Path / Tree_parent };
        const fs::path t_Child{ obj_Path / Path_tree };

        fs::ifstream ifs_2{ t_Parent };
        fs::ifstream ifs_3{ t_Child };

        fs::ofstream ofs{ t_Child, ios::app };

        // On boucle alors que le fichier parent a encore une ligne
        while (getline(ifs_2, parent_Line))
        {
            while (getline(ifs_3, child_Line))
            {
                if (child_Line.find(parent_Line) == string::npos)
                    ofs << parent_Line << endl;
            }
        }
    }
         

    // Faire le commit
    ofs_obj << "parent " << parent << endl << "tree " << tree << endl << d_time << endl << "author " << author << endl << "message " << endl << message << endl;
    o_head << sha1 << endl; // mettre à jour le HEAD

    o_head.close();
    ofs_obj.close();

    cout << "Commit has been successfully established !! :)" << endl;
    return true;
}


/*
* Cette fonction est utilisée pour vérifier si une ligne est déjà écrite ou pas
* Elle prend comme parametres: le chemain et le text à vérifier
* Elle retourne 1 si le text est présent, 0 sinon
*/

bool Verif_File(string path, string txt)
{
    fs::path chemain{ path };
    fs::ifstream ifs{ chemain };
    // Si le chemain est vide ne rien faire 
    if (fs::is_empty(chemain))
        return 0;
    else
    {
        string line;
        while (getline(ifs, line))
            if (line.find(txt) != string::npos)
                return 1;
    }
    return 0;
}

/*
* Cette fonction assure la création de l'arbre
* Elle prend comme parameres: le chemain et la chaine de caractères
* Elle génère finalement l'arbre sha1
*/
string Creat_Tree(string chemain, string txt)
{
    const string myTxt = "tree" + txt;
    const string sha1 = Sha1Generator(myTxt);
    string myData;
    string myFile = Dir_creation(sha1);
    boost::system::error_code code;

    //try
    //{
    //   myFile = Dir_creation(sha1);
    //}
    //catch (exception& e)
    //{
    //    cout << e.what() << endl;
    //}

    // Si le chemin se trouve dans un autre sous-dossier
    if (chemain != ".git/index")
    {
        const fs::path index_path{ ".git/index" };
        fs::ifstream ifs_index{ index_path };

        fs::path myObjects{ myFile };
        fs::ofstream myTree(myObjects, ios::app);

        // Lire les lignes de l'index
        while (getline(ifs_index, myData))
        {
            string myDir = myData.substr(chemain.length(), myData.find("/") - 2);

            if (myData.substr(0, chemain.length()) == chemain) // Vérifier le chemin du sous-dossier
            {
                if (Verif_File(myFile, myData) == false) // Vérifiez que la ligne n'est pas déjà écrite
                {
                    if (fs::is_directory((chemain + myDir), code)) // Vérifiez si la ligne contient un dossier
                    {
                        string subtree = Creat_Tree(chemain + myDir, "subtree" + myDir);  // Créer une arborescence pour le dossier
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
        const fs::path index_path{ chemain };
        fs::ifstream ifs_index{ index_path };

        const fs::path myObjects{ myFile };
        fs::ofstream myTree(myObjects, ios::app);

        while (getline(ifs_index, myData))
        {
            string myDir = myData.substr(0, myData.find("/"));
            if (Verif_File(myFile, myDir) == 0)
            {
                if (fs::is_directory(myDir))
                {
                    string subtree = Creat_Tree(myDir, "subtree" + myDir);
                    myTree << "tree " << myDir << " " << subtree << endl;
                }
                else
                    myTree << "blob " << myData << endl;
            }
        }
        myTree.close();
    }

    const fs::path index{ ".git/index" };
    fs::ofstream ofs_index(index);      // Effacer le fichier d'index

    return sha1;
}
string Dir_creation(string sha1)
{
    boost::system::error_code code;
    const string dir_name = sha1.substr(0, 2);
    const string file_name = sha1.substr(2);
    const auto currentPath = boost::filesystem::current_path();
    auto myPath = currentPath / ".git" / "objects" / dir_name;

    boost::filesystem::create_directory(myPath, code);
    if (!boost::filesystem::exists(myPath, code)) // ne lance pas d'exception .. c'est attrapé par le code
    {
        if (code.failed()) // et ensuite, on vérifie pour voir si le code est ok
        {
            cout << "Something bad happened but we didn't throw anything...so thats good :)" << endl;
            return("");
        }
    }

    boost::filesystem::ofstream(myPath.append(file_name));
    return(myPath.string());
}
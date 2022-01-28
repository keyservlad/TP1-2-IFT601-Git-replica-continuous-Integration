#include "commit.h"
#include "sha1.h"
#include <boost/filesystem.hpp>
#include <ctime>

namespace fs = boost::filesystem;

/*
* Cette fonction est utilis�e pour v�rifier si une ligne est d�j� �crite ou pas
* Elle prend comme parametres: le chemain et le text � v�rifier
* Elle retourne 1 si le text est pr�sent, 0 sinon
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
* Cette fonction assure la cr�ation de l'arbre
* Elle prend comme parameres: le chemain et la chaine de caract�res
* Elle g�n�re finalement l'arbre sha1
*/
std::string Creat_Tree(std::string chemain, std::string txt)
{
    return("xxx");
}

void Add_Parent_File(std::string parent, std::string tree)
{

}

void Commit(std::string msg, std::string author)
{

}
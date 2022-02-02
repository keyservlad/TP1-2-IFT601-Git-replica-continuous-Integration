#include "commit.h"
#include "sha1.h"
#include <ctime>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/*
* Verify that a line is not already written in a file
*   Parameters: (p) File path
*               (l) Text to verify
*   Return:      0 if the line is not present
*                1 if the line is present
*/
bool VerifyFile(std::string pathStr, std::string text)
{
    fs::path path{ pathStr };
    fs::ifstream ifs{ path };

    if (fs::is_empty(path))
        return 0;
    else
    {
        std::string line;
        while (std::getline(ifs, line))
            if (line.find(text) != std::string::npos)
                return 1;
    }

    return 0;
}

/*
* Used to create a tree
*   Parameters: (path) Index path
*               (str) Any std::string
*   Return:     Tree's sha1
*/
std::string CreateTree(std::string path, std::string str)
{
    boost::system::error_code code;
    std::string data;
    std::string sha1 = Sha1Generator("tree" + str);
    std::string file = "";



    const string NAME_DIR = sha1.substr(0, 2);
    const string NAME_FILE = sha1.substr(2);

    auto currentPath = boost::filesystem::current_path();
    boost::filesystem::create_directory(currentPath.append(".git").append("objects").append(NAME_DIR), code);

    boost::filesystem::ofstream(currentPath.append(NAME_FILE));

    file = currentPath.string();


    // If the path is inside another subfolder
    if (path != ".git/index")
    {
        fs::path index{ ".git/index" };
        fs::ifstream ifs{ index };

        fs::path objects{ file };
        fs::ofstream tree(objects, ios::app);

        // Read lines of the index
        while (std::getline(ifs, data))
        {
            std::string dir = data.substr(path.length(), data.find("/") - 2);

            if (data.substr(0, path.length()) == path) // Verify the subfolder path
            {
                if (VerifyFile(file, data) == false) // Verify that the line is not already written
                {
                    if (fs::is_directory((path + dir), code)) // Verify if the line contains a folder
                    {
                        std::string subtree = CreateTree(path + dir, "subtree" + dir);  // Create a tree for the folder
                        tree << "tree " << dir.substr(1) << " " << subtree << endl;
                    }
                    else
                        tree << "blob " << data.substr(path.length() + 1) << endl;
                }
            }
        }
        tree.close();
    }
    else
    {
        fs::path index{ path };
        fs::ifstream ifs{ index };

        fs::path objects{ file };
        fs::ofstream tree(objects, ios::app);

        while (std::getline(ifs, data))
        {
            std::string dir = data.substr(0, data.find("/"));
            if (VerifyFile(file, dir) == 0)
            {
                if (fs::is_directory(dir))
                {
                    std::string subtree = CreateTree(dir, "subtree" + dir);
                    tree << "tree " << dir << " " << subtree << endl;
                }
                else
                    tree << "blob " << data << endl;
            }
        }
        tree.close();
    }

    fs::path index{ ".git/index" };
    fs::ofstream ofs_index(index);      // Clear index file

    return sha1;
}

/*
* Used to add parent files into child tree
*   Parameters: (parent) Sha1 of the parent commit
*               (tree) Sha1 of the child tree
*/
void AddParentFile(std::string parent, std::string tree)
{
    std::string line;
    std::string parentTree;
    auto curPath = fs::current_path();
    std::string parentPath = parent.substr(0, 2) + "/" + parent.substr(2);
    std::string treePath = tree.substr(0, 2) + "/" + tree.substr(2);

    // Read parent to get his tree
    fs::path parentP{ curPath.append(".git/objects/" + parentPath) };
    fs::ifstream ifs{ parentP };

    while (std::getline(ifs, line))
        if (line.substr(0, 4).find("tree") != std::string::npos)
            parentTree = line.substr(5, 2) + "/" + line.substr(7);

    std::string lineParent;
    std::string lineChild;

    auto curPath2 = fs::current_path();
    auto curPath3 = fs::current_path();

    fs::path parentT{ curPath2.append(".git/objects/" + parentTree) };
    fs::path childT{ curPath3.append(".git/objects/" + treePath) };

    fs::ifstream ifs2{ parentT };
    fs::ifstream ifs3{ childT };

    fs::ofstream ofs{ childT, ios::app };

    // We loop while the parent file still have a line
    while (std::getline(ifs2, lineParent))
    {
        while (std::getline(ifs3, lineChild))
        {
            if (lineChild.find(lineParent) == std::string::npos)
                ofs << lineParent << endl;
        }
    }
}

bool Commit(std::string msg, std::string author)
{
    std::string commitParent = "\0";
    std::string sha1 = Sha1Generator("commit" + msg + author);

    boost::system::error_code code;
    const string NAME_DIR = sha1.substr(0, 2);
    const string NAME_FILE = sha1.substr(2);

    auto currentPath = boost::filesystem::current_path();
    boost::filesystem::create_directory(currentPath.append(".git").append("objects").append(NAME_DIR), code);

    boost::filesystem::ofstream(currentPath.append(NAME_FILE));

    std::string file = currentPath.string();


    time_t now = time(0);
    char* dt = ctime(&now);


    std::cout << file << std::endl;
    fs::path head{ ".git/HEAD" };
    fs::path objects{ file };

    fs::ifstream ifs_head{ head };

    if (!fs::is_empty(head))
        ifs_head >> commitParent;

    fs::ofstream ofs_head(head);
    fs::ofstream ofs(objects);

    std::string tree = CreateTree(".git/index", "tree" + msg + author); // Create tree

    if (commitParent != "\0")
        AddParentFile(commitParent, tree); // Add parent files already present

    // Write commit file
    ofs << "parent " << commitParent << endl << "tree " << tree << endl << dt << endl << "author " << author << endl << "author " << endl << msg << endl;
    ofs_head << sha1 << endl; // Update HEAD

    ofs_head.close();
    ofs.close();

    std::cout << "Commit successfull" << std::endl;
}
#ifndef COMMIT_H   
#define COMMIT_H
#include <iostream> 

bool Verif_File(std::string p, std::string l); // AB - les noms des param sont pas supers
std::string Creat_Tree(std::string chemain, std::string txt); // AB - create...with an E
bool Commit(std::string msg, std::string author);
std::string Dir_creation(std::string sha1);

#endif


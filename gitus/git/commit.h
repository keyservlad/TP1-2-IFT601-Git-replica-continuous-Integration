#ifndef COMMIT_H   
#define COMMIT_H
#include <iostream> 

bool Verif_File(std::string p, std::string l);
std::string Creat_Tree(std::string chemain, std::string txt);
bool Commit(std::string msg, std::string author);
std::string Dir_creation(std::string sha1);

#endif


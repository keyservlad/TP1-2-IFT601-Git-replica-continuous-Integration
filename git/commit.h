#ifndef COMMIT_H   
#define COMMIT_H
#include <iostream> 

void Add_Parent_File(std::string parent, std::string tree);
bool Verif_File(std::string p, std::string l);
std::string Creat_Tree(std::string chemain, std::string txt);
void Commit(std::string msg, std::string author);

#endif


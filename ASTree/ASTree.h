#ifndef ASTree_H
#define ASTree_H
//////////////////////////////////////////////////////////////////
// ASTree.h - M-ary Tree data structure                         //
//                                                              //
// Application: Project 2 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Source:      Jim Fawcett, CST 4-187, 443-3948                //
//              jfawcett@twcny.rr.com                           //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains classes ASTree and ASTNode.
* ASTree is an M-ary tree, e.g., each ASTNode has zero or more
* children.  Note that this is not a balanced binary tree.  No
* order is imposed on the children of any node.  They simply appear
* in the order they were added.
*
* M-ary trees are often used to represent parse trees where the
* ordering of nodes depends on the structure of the entity being
* represented, e.g., source code, XML, or HTML.
*
* ASTree's nodes are members of class ASTNode.  each ASTNode
* holds a vector of pointers to it's children, if any.
*
* Required Files:
* ---------------
*   - ASTNode.h, ASTNode.cpp, ASTree.h, ASTree.cpp
*
* Build Process:
* --------------
*   devenv ASTree.sln /debug rebuild
*
* Maintenance History:
* --------------------
* Ver 1 : 29 Feb 16
*
*/

#include "ASTNode.h"

// ASTree class

class ASTree
{
public:
  ASTree();
  ASTree(ASTNode*);
  ~ASTree();
  ASTNode* root();
  ASTNode* createNode();
  ASTNode* currNode();
  void DFS(ASTNode * node);
  void addChildNode(ASTNode * toAdd);
  void backUpNode();
private:
  ASTNode* pRoot_;
  ASTNode* currNode_;
  ASTNode* parNode_;
};

#endif
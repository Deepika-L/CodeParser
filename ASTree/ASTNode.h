#ifndef ASTNode_H
#define ASTNode_H
//////////////////////////////////////////////////////////////////
// ASTNode.h - Node helper for M-ary Tree data structure        //
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

#include <vector>
#include <string>

// ASTNode<T> class

class ASTNode {
public:
  ASTNode();
  ~ASTNode();
  bool addChild(ASTNode* toAdd);
  size_t size() const;
  std::string showchildren();
  std::string show();
  void setData(std::string sname, std::string stype, size_t startLine);
  void setEndLine(size_t endLine);
  void setParent(ASTNode * ptr);
  std::vector<ASTNode*> nodeChildren();
  ASTNode* pPtr();
  size_t returnEndLine();
  std::string returnType();
  std::string returnName();
private:
  std::string type;
  std::string name;
  size_t startLineCount = 0;
  size_t endLineCount = 0;
  ASTNode* parent = this;
  std::vector<ASTNode*> children_;
};

#endif

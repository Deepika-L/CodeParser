//////////////////////////////////////////////////////////////////
// ASTree.cpp - M-ary Tree data structure                       //
//                                                              //
// Application: Project 2 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Source:      Jim Fawcett, CST 4-187, 443-3948                //
//              jfawcett@twcny.rr.com                           //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////

#include "ASTNode.h"
#include "ASTree.h"
#include <string>
#include <iostream>

// Construct tree and initialize values
ASTree::ASTree() {
  pRoot_ = ASTree::createNode();
  pRoot_->setData("Global namespace", "namespace", 1);
  currNode_ = pRoot_;
  parNode_ = pRoot_;
}

//Construct tree if pointer to root node is passed
ASTree::ASTree(ASTNode* pRoot) : pRoot_(pRoot), currNode_(pRoot), parNode_(pRoot) {
}

//Destructor for ASTree
ASTree::~ASTree() {
}

// Return tree root pointer
ASTNode* ASTree::root() { return pRoot_; }

// Return current node pointer
ASTNode* ASTree::currNode() { return currNode_; }

//Create new tree node
ASTNode* ASTree::createNode() {
  ASTNode* newNode = new ASTNode;
  return newNode;
}

//Add child node to current node
void ASTree::addChildNode(ASTNode* toAdd) {
  toAdd->setParent(parNode_);
  currNode_->addChild(toAdd);
  parNode_ = currNode_;
  currNode_ = toAdd;
}

// Move current node pointer one level up
void ASTree::backUpNode() {
  ASTNode* temp = currNode_->pPtr();
  currNode_ = parNode_;
  parNode_ = temp;
}

// Traverse the AST and display using DFS
void ASTree::DFS(ASTNode* node)
{
  static size_t indentLevel = 0;
  std::cout << "\n  " << std::string(4 * indentLevel, ' ') << node->show() << "\n";
  ++indentLevel;
  for (ASTNode* pChild : node->nodeChildren())
  {
    DFS(pChild);
  }
  --indentLevel;
}

//--------------------------------------Test stub----------------------------------------
#ifdef TEST_ASTREE

int main()
{
  ASTNode* rootNode = new ASTNode;
  rootNode->setData("string", "root of the tree", 0);
  ASTree testTree(rootNode);
  std::cout << "Created tree root\n";
  ASTNode* branch1 = new ASTNode;
  branch1->setData("string", "branch 1", 1);
  testTree.addChildNode(branch1);
  ASTNode* child1 = new ASTNode;
  child1->setData("string", "child 1", 2);
  testTree.addChildNode(child1);
  testTree.backUpNode();
  ASTNode* child2 = new ASTNode;
  child2->setData("string", "child 2", 3);
  testTree.addChildNode(child2);
  testTree.backUpNode();
  testTree.backUpNode();
  ASTNode* branch2 = new ASTNode;
  branch2->setData("string", "branch 2", 4);
  testTree.addChildNode(branch2);
  ASTNode* child21 = new ASTNode;
  child21->setData("string", "child 1", 5);
  testTree.addChildNode(child21);
  testTree.backUpNode();
  ASTNode* child22 = new ASTNode;
  child22->setData("string", "child 2", 6);
  testTree.addChildNode(child22);
  std::cout << "Created children\n";
  std::cout << "DFS starting at tree root: \n";
  testTree.DFS(rootNode);
  std::cout << "\n\n";
  std::cout << "DFS starting at branch2: \n";
  testTree.DFS(branch2);
  std::cout << "\n\n";
}

#endif
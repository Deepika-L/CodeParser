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

#include "ASTNode.h"
#include <string>
#include <iostream>
#include <sstream>

// Default constructor
ASTNode::ASTNode()
{ }

// Destructor
ASTNode::~ASTNode()
{ }

// Return end line count
size_t ASTNode::returnEndLine()
{
  return endLineCount;
}

// Return type
std::string ASTNode::returnType()
{
  return type;
}

// Return name
std::string ASTNode::returnName()
{
  return name;
}

// Access children of node
std::vector<ASTNode*> ASTNode::nodeChildren()
{
  return children_;
}

// Add child node to children vector
bool ASTNode::addChild(ASTNode* toAdd)
{
  children_.push_back(toAdd);
  return true;
}

// Set data elements to node
void ASTNode::setData(std::string sname, std::string stype, size_t startLine)
{
  name = sname;
  type = stype;
  startLineCount = startLine;
}

// Set end line count of node
void ASTNode::setEndLine(size_t endLine)
{
  endLineCount = endLine;
}

// Set parent pointer
void ASTNode::setParent(ASTNode* ptr)
{
  parent = ptr;
}

// Return number of children for node
size_t ASTNode::size() const {
  return children_.size();
}

// Return parent pointer
ASTNode* ASTNode::pPtr() {
  return parent;
}

// Function to show children of node
std::string ASTNode::showchildren() {
  std::ostringstream temp;
  for (ASTNode* child : children_) {
    temp << child->show() << "\n";
  }
  return temp.str();
}

//Function to display contents of node
std::string ASTNode::show()
{
  std::ostringstream temp;
  temp << "( Type: ";
  temp << type;
  temp << ", Name: ";
  temp << name;
  temp << ", Start line: ";
  temp << startLineCount;
  temp << ", Number of lines: ";
  temp << endLineCount;
  temp << " )";
  return temp.str();
}


//--------------------------------------Test stub----------------------------------------
#ifdef TEST_ASTNODE

int main()
{
  std::cout << "Testing ASTNode class\n";
  ASTNode testNode;
  testNode.setData("string", "parent", 0);
  std::cout << "Creating parent\n";
  ASTNode addChild1;
  addChild1.setData("string", "first", 1);
  testNode.addChild(&addChild1);
  std::cout << "Adding first child\n";
  ASTNode addChild2;
  addChild2.setData("string", "second", 2);
  testNode.addChild(&addChild2);
  std::cout << "Adding second child\n";

  std::cout << "Displaying node info: \n";
  std::cout << testNode.show() << "\n\n";
  std::cout << "Displaying children info: \n";
  std::cout << testNode.showchildren() << "\n\n";
}

#endif
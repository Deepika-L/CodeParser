//////////////////////////////////////////////////////////////////
// MetricsAnalysis.h - Metric Analyser package                  //
//                                                              //
// Application: Project 2 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////

#include <iostream>
#include "MetricsAnalysis.h"

// Sets off analysis on the AST
void MetricsAnalysis::runAnalysis() {
  MAnalysis(tree);
}

// Begins analysis at ASTNode passed to it
void MetricsAnalysis::MAnalysis(ASTNode* node) {
  int num = 0;
  MAnalysisHelper(node, num);
  if (node->returnType() == "function") {
    std::cout << "\n Complexity of function " << node->returnName() << ": " << num + 1;
    std::cout << "\n Number of lines: " << node->returnEndLine();
    std::cout << "\n\n";
  }
  for (ASTNode* pChild : node->nodeChildren())
  {
    MAnalysis(pChild);
  }
}

// Helper function for MetricsAnalysis
void MetricsAnalysis::MAnalysisHelper(ASTNode* node, int& n) {
  for (ASTNode* pChild : node->nodeChildren())
  {
    n++;
    MAnalysisHelper(pChild, n);
  }
}

//--------------------------------------Test stub----------------------------------------
#ifdef TEST_METRICANALYZER
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
  child1->setData("child 1", "function", 2);
  testTree.addChildNode(child1);
  testTree.backUpNode();
  ASTNode* child2 = new ASTNode;
  child2->setData("child 2", "function", 3);
  testTree.addChildNode(child2);
  testTree.backUpNode();
  testTree.backUpNode();
  ASTNode* branch2 = new ASTNode;
  branch2->setData("string", "branch 2", 4);
  testTree.addChildNode(branch2);
  ASTNode* child21 = new ASTNode;
  child21->setData("child 1", "function", 5);
  testTree.addChildNode(child21);
  testTree.backUpNode();
  ASTNode* child22 = new ASTNode;
  child22->setData("string", "child 2", 6);
  testTree.addChildNode(child22);
  testTree.DFS(rootNode);
  MetricsAnalysis testM(rootNode);
  testM.runAnalysis();
}
#endif
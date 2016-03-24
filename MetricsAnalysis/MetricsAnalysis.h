#ifndef METRICS_ANALYSIS
#define METRICS_ANALYSIS
//////////////////////////////////////////////////////////////////
// MetricsAnalysis.h - Metric Analyser package                  //
//                                                              //
// Application: Project 2 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains the Metric Analyzer package for evaluating
* and displaying the size and complexity of all global functions,
* static member functions, and non-static member functions in each 
* of a set of specified packages. 
* The complexity is computed from the Abstract Syntax Tree of each 
* file.
*
* Required Files:
* ---------------
*   ASTree.h ASTree.cpp ASTNode.h ASTNode.cpp
*
* Build Process:
* --------------
*   devenv MetricsAnalysis.sln ASTNode.cpp ASTree.cpp /debug rebuild
*
* Maintenance History:
* --------------------
* Ver 1 : 29 Feb 16
*/
#include <iostream>
#include <string>
#include "../ASTree/ASTree.h"
#include  "../ASTree/ASTNode.h"

class MetricsAnalysis {
  ASTNode* tree;
  
public:
  MetricsAnalysis() {}
  MetricsAnalysis(ASTNode* root) {
    tree = root;
  }
  void runAnalysis();
  void MAnalysis(ASTNode* node);
  void MAnalysisHelper(ASTNode* node, int& n);
};

#endif
#ifndef METRICS_EXECUTIVE
#define METRICS_EXECUTIVE
//////////////////////////////////////////////////////////////////
// MetricsExecutive.h - Metric Executive package                //
//                                                              //
// Application: Project 2 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains the Metric Executive that enables collecting
* metrics for all the packages with names that match a specified pattern 
* in a directory tree rooted at a specified path. The path and pattern 
* are defined on the command line.
*
* Required Files:
* ---------------
*   - Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp,
*     tokenizer.h, tokenizer.cpp,
*     ActionsAndRules.h, ActionsAndRules.cpp,
*     ConfigureParser.h, ConfigureParser.cpp,
*     ASTree.h ASTree.cpp,
*     ASTNode.h, ASTNode.cpp
*     MetricsAnalysis.h, MetricsAnalysis.cpp
*
* Build Process:
* --------------
*   devenv MetricsExecutive.sln devenv Parser.sln parser.cpp semiexpression.cpp tokenizer.cpp
*   ActionsAndRules.cpp ConfigureParser.cpp ASTree.cpp ASTNode.cpp MetricsAnalysis.cpp /debug rebuild
*
* Maintenance History:
* --------------------
* Ver 1 : 29 Feb 16
*/

class MetricsExecutive {
public:
  void getAllFiles(std::string, std::vector<std::string>*, std::string);
  void getDirFiles(std::string, std::vector<std::string>*, std::string);
  int parseFile(std::string);
};
#endif
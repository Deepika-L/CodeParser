//////////////////////////////////////////////////////////////////
// MetricsExecutive.cpp - Metric Executive package              //
//                                                              //
// Application: Project 2 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <io.h>
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../MetricsAnalysis/MetricsAnalysis.h"
#include "MetricsExecutive.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

using namespace FileSystem;

// Recursively get all files from root path
void MetricsExecutive::getAllFiles(std::string sDir, std::vector<std::string>* files, std::string fPattern)
{
  // Find the subfolders in the folder that is passed in.
  std::vector<std::string> d = Directory::getDirectories(sDir);
  size_t numDirs = d.size();

  for (size_t i = 2; i < numDirs; i++)
  {
    // Find all the files in the subfolder.
    std::vector<std::string> f = Directory::getFiles(sDir + "\\" + d[i], fPattern);
    size_t numFiles = f.size();

    for (int j = 0; j < numFiles; j++)
    {
      files->push_back(sDir + "\\" + d[i] + "\\" + f[j]);
    }
    getAllFiles(sDir + "\\" + d[i], files, fPattern);
  }
}

// Get all files in given path
void MetricsExecutive::getDirFiles(std::string sDir, std::vector<std::string>* files, std::string fPattern) {
  std::vector<std::string> f = Directory::getFiles(Path::getFullFileSpec(sDir), fPattern);
  size_t numFiles = f.size();
  for (int j = 0; j < numFiles; j++)
  {
    files->push_back(Path::getFullFileSpec(sDir) + "\\" + f[j]);
  }
}

// Run Parser and Metric Analyzer on given file
int MetricsExecutive::parseFile(std::string fileSpec) {
  ConfigParseToConsole configure;
  Parser* pParser = configure.Build();
  try
  {
    if (pParser)
    {
      if (!configure.Attach(fileSpec))
      {
        std::cout << "\n  Could not open file " << fileSpec << std::endl;
        return 1;
      }
    }
    else
    {
      std::cout << "\n\n  Parser not built\n\n";
      return 1;
    }
    // Now that parser is built, use it
    std::cout << " PARSER OUTPUT: \n";

    while (pParser->next())
      pParser->parse();

    std::cout << "\n";
    Repository* currRepo = configure.Repo();
    ASTree& treeRoot = currRepo->ast();
    ASTNode* root = treeRoot.root();
    root->setEndLine(currRepo->lineCount());
    std::cout << "\n\n---------------------------------------------------------------------------------------------\n\n";
    std::cout << " TREE WALK OF AST: \n\n";
    currRepo->doDFS(root);
    std::cout << "\n\n---------------------------------------------------------------------------------------------\n\n";
    std::cout << " COMPLEXITY ANALYSIS OF FUNCTIONS IN AST:\n\n";
    MetricsAnalysis mAnal(root);
    mAnal.runAnalysis();
    return 0;
  }
  catch (std::exception& ex)
  {
    std::cout << "\n\n    " << ex.what() << "\n\n";
    return 1;
  }
}

//--------------------------------------Test stub--------------------------------------------
#ifdef TEST_METRICSEXECUTIVE
int main (int argc, char* argv[])
{
  MetricsExecutive test;
  if (argc < 2)
  {
    std::cout << "\n  please enter root path and filename pattern to process on command line\n\n";
    return 1;
  }
  std::string dir_path = argv[1];
  if (_access(dir_path.c_str(), 0) != 0)
    dir_path = "." + dir_path;

  std::vector<std::string> files;

  // Get all files from current directory
  for (int i = 2; i < argc; ++i) {
    test.getDirFiles(Path::getFullFileSpec(dir_path), &files, argv[i]);
  }

  //Get all files from sub-directories
  for (int i = 2; i < argc; ++i) {
    test.getAllFiles(Path::getFullFileSpec(dir_path), &files, argv[i]);
  }

  // Parse list of files
  for (auto fileSpec : files)
  {
    std::cout << "\n DEMONSTRATING PROJECT 2 REQUIREMENTS\n\n";
    std::cout<< " PROCESSING FILE: " << fileSpec;
    std::cout << "\n\n";

    test.parseFile(fileSpec);
    
    std::cout << "\n\n===============================================================================================\n\n";
  }
  std::cout << "\n";
}
#endif

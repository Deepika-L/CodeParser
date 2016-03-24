//////////////////////////////////////////////////////////////////
// Parser.cpp - Analyzes C++ language constructs                //
//                                                              //
// Application: Project 2 for CIS 687, Spring 2016              //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015 //
// Source:      Jim Fawcett, CST 4-187, 443-3948                //
//              jfawcett@twcny.rr.com                           //
// Author:      Deepika Lakshmanan                              //
//              dlakshma@syr.edu                                //
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
using namespace Utilities;

// Register parsing rule
void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}

// Get next ITokCollection
bool Parser::next()
{
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if (!succeeded)
    return false;
  return true;
}

// Parse the SemiExp by applying all rules to it
bool Parser::parse()
{
  bool succeeded = false;
  for (size_t i = 0; i < rules.size(); ++i)
  {
    if (rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}

// Register action with a rule
void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}

// Invoke all actions associated with a rule
void IRule::doActions(ITokCollection*& pTokColl)
{
  if (actions.size() > 0)
    for (size_t i = 0; i < actions.size(); ++i)
      actions[i]->doAction(pTokColl);
}


//--------------------------------------Test stub----------------------------------------

#ifdef TEST_PARSER

#include <queue>
#include <string>
#define Util StringHelper
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

int main(int argc, char* argv[])
{
  Util::Title("Testing Parser Class", '=');
  putline();
  // collecting tokens from files, named on the command line
  if (argc < 2)
  {
    std::cout << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for (int i = 1; i < argc; ++i)
  {
    std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
    std::string msg = "Processing file" + fileSpec;
    Util::title(msg);
    putline();

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if (pParser)
      {
        if (!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << fileSpec << std::endl;
          continue;
        }
      }
      else {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it
      while (pParser->next())
        pParser->parse();
      std::cout << "\n";
    }
    catch (std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
  }
  std::cout << "\n";
}

#endif
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers              //
//  ver 1.0                                                        //
//                                                                 //
//  Application: Project 2 for CIS 687, Spring 2016                //
//  Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015   //
//  Source:      Jim Fawcett, CST 4-187, 443-3948                  //
//               jfawcett@twcny.rr.com                             //
//  Author:      Deepika Lakshmanan                                //
//               dlakshma@syr.edu                                  //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "../ASTree/ASTree.h"

using namespace Scanner;

//----< Destructor releases all parts >------------------------------
ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pAnyScope;
  delete pArrayScope;
  delete pDeclarationScope;
  delete pPushArrayScope;
  delete pPushDeclarationScope;
  delete pPushScope;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  pIn->close();
  delete pIn;
}

//----< Attach toker to a file stream or stringstream >------------
bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}

// Return pointer to the repository
Repository* ConfigParseToConsole::Repo()
{
  return pRepo;
}

//----< Here's where alll the parts get assembled >----------------
Parser* ConfigParseToConsole::Build() {
  try {
    // add Parser's main parts
    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
    // configure to manage scope - these must come first - they return true on match so rule checking continues
    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);
    // configure to detect and act on function definitions - these will stop further rule checking by returning false
    pFunctionDefinition = new FunctionDefinition;
    pAnyScope = new AnyScope;
    pDeclarationScope = new DeclarationScope();
    pPushDeclarationScope = new PushDeclarationScope(pRepo);
    pPushScope = new PushScope(pRepo);
    pPushFunction = new PushFunction(pRepo);
    pPrintFunction = new PrintFunction(pRepo);
    pAnyScope->addAction(pPushScope);
    pFunctionDefinition->addAction(pPushFunction);
    pArrayScope = new ArrayScope;
    pPushArrayScope = new PushArrayScope(pRepo);
    pArrayScope->addAction(pPushArrayScope);
    pFunctionDefinition->addAction(pPrintFunction);
    pParser->addRule(pFunctionDefinition);
    pParser->addRule(pDeclarationScope);
    pParser->addRule(pArrayScope);
    pParser->addRule(pAnyScope);
    pDeclarationScope->addAction(pPushDeclarationScope);
    // configure to detect and act on declarations and Executables
    pDeclaration = new Declaration;
    pShowDeclaration = new ShowDeclaration;
    pDeclaration->addAction(pShowDeclaration);
    pParser->addRule(pDeclaration);
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
    pExecutable->addAction(pShowExecutable);
    pParser->addRule(pExecutable);
    return pParser; }
  catch(std::exception& ex) {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0; }
}


//--------------------------------------Test stub----------------------------------------
#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif

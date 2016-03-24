/////////////////////////////////////////////////////////////////////
// ActionsAndRules.cpp - declares new parsing rules and actions      //
// ver 1.0                                                         //
// Application: Project 2 for CIS 687, Spring 2016                 //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015    //
// Source:      Jim Fawcett, CST 4-187, 443-3948                   //
//              jfawcett@twcny.rr.com                              //
// Author:      Deepika Lakshmanan                                 //
//              dlakshma@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

//--------------------------------------Test stub----------------------------------------

#ifdef TEST_ACTIONSANDRULES

#include <iostream>
#include "ActionsAndRules.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/itokcollection.h"
#include "Parser.h"
#include <fstream>
using namespace Scanner;
using namespace Utilities;

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ActionsAndRules class\n "
            << std::string(30,'=') << std::endl;

  try
  {
    std::queue<std::string> resultsQ;
    PrintPreproc ppq;
    PreprocStatement pps;
    pps.addAction(&ppq);

    FunctionDefinition fnd;
    PrettyPrintFunction pprtQ;
    fnd.addAction(&pprtQ);
    std::string file = "ActionsAndRules.h";
    std::ifstream in(file);
    if (!in.good())
    {
      std::cout << "\n  can't open " << file << "\n\n";
      getchar();
      return 1;
    }
    Toker toker;
    toker.attach(&in);
    SemiExp se(&toker);
    Parser parser(&se);
    parser.addRule(&pps);
    parser.addRule(&fnd);
    while(se.get())
      parser.parse();
    size_t len = resultsQ.size();
    for(size_t i=0; i<len; ++i)
    {
      std::cout << "\n  " << resultsQ.front().c_str();
      resultsQ.pop();
    }
    std::cout << "\n\n";
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
}
#endif

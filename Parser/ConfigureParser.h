#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 1.0                                                        //
//                                                                 //
//  Application: Project 2 for CIS 687, Spring 2016                //
//  Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015   //
//  Source:      Jim Fawcett, CST 4-187, 443-3948                  //
//               jfawcett@twcny.rr.com                             //
//  Author:      Deepika Lakshmanan                                //
//               dlakshma@syr.edu                                  //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
      ASTree.h ASTree.cpp, ASTNode.h, ASTNode.cpp

  Build command:
    - devenv Parser.sln parser.cpp semiexpression.cpp tokenizer.cpp
      ActionsAndRules.cpp ConfigureParser.cpp ASTree.cpp ASTNode.cpp /debug rebuild

  Maintenance History:
  ====================
  ver 1.0 : 29 Feb 16

*/

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "../ASTree/ASTree.h"

// Build parser that writes its output to console
class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  Repository * Repo();
  Parser* Build();

private:
  // Builder must hold onto all the pieces

  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;

  // Add Rules and Actions

  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;
  PrintFunction* pPrintFunction;
  Declaration* pDeclaration;
  ShowDeclaration* pShowDeclaration;
  Executable* pExecutable;
  ShowExecutable* pShowExecutable;

  AnyScope* pAnyScope;
  PushScope* pPushScope;
  DeclarationScope* pDeclarationScope;
  PushDeclarationScope* pPushDeclarationScope;
  ArrayScope* pArrayScope;
  PushArrayScope* pPushArrayScope;

  // Prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&) = delete;
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};

#endif

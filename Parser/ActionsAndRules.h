#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
// ActionsAndRules.h - declares new parsing rules and actions      //
// ver 1.0                                                         //
// Application: Project 2 for CIS 687, Spring 2016                 //
// Platform:    HP dv6 Notebook, Windows 10, Visual Studio 2015    //
// Source:      Jim Fawcett, CST 4-187, 443-3948                   //
//              jfawcett@twcny.rr.com                              //
// Author:      Deepika Lakshmanan                                 //
//              dlakshma@syr.edu                                   //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This module defines several action classes.  Its classes provide
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable.

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
      ASTree.h ASTree.cpp, ASTNode.h, ASTNode.cpp
  Build command:
    - devenv Parser.sln parser.cpp semiexpression.cpp tokenizer.cpp
      ActionsAndRules.cpp ConfigureParser.cpp ASTree.cpp ASTNode.cpp /debug rebuild

  Maintenance History:
  ====================
  ver 1.0 : 29 Feb 16
*/

#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../ASTree/ASTree.h"


// ScopeStack element is application specific
struct element
{
  std::string type;
  std::string name;
  size_t lineCount;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << lineCount;
    temp << ")";
    return temp.str();
  }
};


// Repository instance is used to share resources among all actions
class Repository
{
  ScopeStack<element> stack;
  Scanner::Toker* p_Toker;
  ASTree AST;
public:
  Repository(Scanner::Toker* pToker)
  {
    p_Toker = pToker;
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }

  ASTree& ast()
  {
    return AST;
  }

  void doDFS(ASTNode* startDFS)
  {
    AST.DFS(startDFS);
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }
};


// Rule to detect beginning of anonymous scope
class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if (pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

// Action to handle scope stack at beginning of scope
class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};

// Rule to detect end of scope
class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if (pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

// Action to handle scope stack and AST at end of scope
class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    if (p_Repos->scopeStack().size() == 0)
      return;

    element elem = p_Repos->scopeStack().pop();

    p_Repos->ast().currNode()->setEndLine(p_Repos->lineCount() - elem.lineCount + 1);
    p_Repos->ast().backUpNode();

    if (elem.type == "function")
    {
      std::cout << "\n  Function " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
      std::cout << "\n";
    }
  }
};

// Rule to detect preprocessor statements
class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if (pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

// Action to print preprocessor statement to console
class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

// Rule to detect function definitions
class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i < 5; ++i)
      if (tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]) && tc.find("=") == tc.length() && len < tc.find("["))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

// Action to add function name onto ScopeStack and to AST
class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);

    // create child node
    ASTNode* node = new ASTNode;
    node->setData(name, "function", p_Repos->lineCount());
    p_Repos->ast().addChildNode(node);
  }
};

// Rule to detect declaration scope
class DeclarationScope : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string names[]
      = { "class", "namespace", "struct" };
    for (int i = 0; i < 3; ++i)
      if (tok == names[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("{");
      if (isSpecialKeyWord(tc[0]))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

// Rule to detect array scope
class ArrayScope : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string names[]
      = { "class", "namespace", "struct" , "char", "std", "string", "float", "int" };
    for (int i = 0; i < 8; ++i)
      if (tok == names[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("{");
      if (isSpecialKeyWord(tc[0]) && tc.find("=") < tc.length())
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

// Action to add array scope to AST
class PushArrayScope : public IAction
{
  Repository* p_Repos;
public:
  PushArrayScope(Repository* pRepos)
  {
    p_Repos = pRepos;
  }

  size_t isSpecialKeyWord(ITokCollection& tc)
  {
    const static std::string names[]
      = { "class", "namespace", "struct" , "int", "char", "std", "string", "float" };
    for (size_t j = 0; j < tc.length(); ++j)
      for (size_t i = 0; i < 8; ++i)
        if (tc[j] == names[i])
          return j;
    return 0;
  }

  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    size_t pos = isSpecialKeyWord(*pTc);
    std::string name = (*pTc)[pos + 1];
    std::string type = (*pTc)[pos] + " array";
    ASTNode* node = new ASTNode;
    node->setData(name, type, p_Repos->lineCount());

    // add to AST
    p_Repos->ast().addChildNode(node);
  }
};

// Action to add declaration scope to AST
class PushDeclarationScope : public IAction
{
  Repository* p_Repos;
public:
  PushDeclarationScope(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  size_t isSpecialKeyWord(ITokCollection& tc)
  {
    const static std::string names[]
      = { "class", "namespace", "struct" };
    for (size_t j = 0; j < tc.length(); ++j)
      for (size_t i = 0; i < 3; ++i)
        if (tc[j] == names[i])
          return j;
    return 0;
  }
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    size_t pos = isSpecialKeyWord(*pTc);

    ASTNode* node = new ASTNode;
    node->setData((*pTc)[pos + 1], (*pTc)[pos], p_Repos->lineCount());
    p_Repos->ast().addChildNode(node);
  }
};

// Rule to detect control structure scope
class AnyScope : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string decs[]
      = { "for", "while", "switch", "if", "catch", "else", "try" ,"[" };
    for (int i = 0; i < 8; ++i)
      if (tok == decs[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if (tc[tc.length() - 1] == "{")
    {
      size_t len = tc.find("(");
      if (isSpecialKeyWord(tc[0]))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

// Action to add control structure scope to AST
class PushScope : public IAction
{
  Repository* p_Repos;
public:
  PushScope(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    std::string name = (*pTc)[0];
    std::string type = "control structure";
    if (name == "[")
      type = "lambda";

    ASTNode* node = new ASTNode;
    node->setData(name, type, p_Repos->lineCount());
    p_Repos->ast().addChildNode(node);
  }
};

// Action to send semi-expression that starts a function def to console
class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};

// Action to send signature of a function def to console
class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    size_t len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for (size_t i = 0; i < len + 1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

// Rule to detect declaration
class Declaration : public IRule
{
public:
  bool isModifier(const std::string& tok)
  {
    const size_t numKeys = 22;
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i < numKeys; ++i)
      if (tok == keys[i])
        return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i < 5; ++i)
      if (tok == keys[i])
        return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i < in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]) || se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

//Action to show declaration
class ShowDeclaration : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
    std::cout << "\n  Declaration: " << se.show();
  }
};

// Rule to detect executable
class Executable : public IRule
{
public:
  bool isModifier(const std::string& tok)
  {
    const size_t numKeys = 22;
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i < numKeys; ++i)
      if (tok == keys[i])
        return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }

  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i < 5; ++i)
      if (tok == keys[i])
        return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }

  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i < in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]) || se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

// Action to show executable
class ShowExecutable : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    std::cout << "\n  Executable: " << se.show();
  }
};

#endif

# CodeParser
Code analysis consists of extracting lexical content from source code files, analyzing the code's syntax from its lexical content, and building an Abstract Syntax Tree (AST) that holds the results of our analysis. It is then fairly easy to build several backends that can do further analyses on the AST to construct code metrics, search for particular constructs, or some other interesting features of the code.

In this second project we will build a Parser-based code analyzer in C++ that consists of at least these packages:

Tokenizer:
extracts words, called tokens, from a stream of characters. Token boundaries are white-space characters, transitions between alphanumeric and punctuator characters, and comment and string boundaries. Certain classes of punctuator characters belong to single character or two character tokens so they require special rules for extraction.

SemiExpression:
groups tokens into sets, each of which contain all the information needed to analyze some grammatical construct without containing extra tokens that have to be saved for subsequent analyses. SemiExpressions are determined by special terminating characters: semicolon, open brace, closed brace, newline when preceeded on the same line with '#', and colon when preceded by one of the three tokens "public", "protected", or "private".

Parser:
Uses Rules to identify specific syntactical constructs and builds an Abstract Syntax Tree to hold that information during analyses. The Parser is a class that contains instances of Rule classes and passes token collections it acquires from ITokenCollection to each rule in turn until there are no more token collections to process. Rules are containers for Actions that are classes that contain methods doAction(const SemiExp& se) that use se to add new elements to the AST.

RulesAndActions:
A package that contains definitions of the Rules and Actions used by Parser. This package will be modified for each type of code analysis implemented by an application.

AbstractSyntaxTree:
A package that provides functionality for the actions to build an Abstract Syntax Tree. This package provides an interface for building and for extracting scope information from the tree.

Metric Analysis and Metric Executive:
Application specific packages that uses information stored in the AST to create a display of all the function and method sizes and complexities of each source code package identified for analysis.

FileMgr:
A package that navigates through a directory tree rooted at some specified path and returns names of all the files and/or directories matching a pattern.
